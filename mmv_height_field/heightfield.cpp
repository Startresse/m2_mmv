#include "heightfield.h"


/* CONSTRUCTORS */

HeightField::HeightField(const cv::Mat& im, const Box2& b, double low, double high)
    : SF2(Grid2(b, im.rows, im.cols)), low(low), high(high)
{
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            at(i, j) = (high - low)*im.at<uchar>(i, j)/img_max_value + low;
            assert(at(i, j) >= low && at(i, j) <= high);
        }
    }
}

HeightField::HeightField(const HeightField& hf) : SF2(static_cast<SF2>(hf))
{
    low = hf.low;
    high = hf.high;
}


/* PROPERTIES */

double HeightField::height(int i, int j) const
{
    return at(i, j);
}

double HeightField::slope(int i, int j) const
{
    return Gradient(i, j).length();
}


double HeightField::average_slope(int i, int j) const
{
    float avg_slp = 0.0;
    int count = 0;
    for (int ii = -1; ii <= 1; ++ii) {
        for (int jj = -1; jj <= 1; ++jj) {
            if (!inside(i + ii, j + jj) || (ii == 0 && jj == 0))
                continue;

            count++;
            avg_slp += abs(slope(i + ii, j + jj));
        }
    }

    avg_slp /= count;
    return avg_slp;
}


QVector3D HeightField::vertex(int i, int j) const
{
    QVector2D xy = Grid2::vertex(i, j);
    return QVector3D(xy.x(), xy.y(), height(i, j));
}

QVector3D HeightField::normal(int i, int j) const
{
    QVector2D grad = Gradient(i, j);
    return QVector3D(-grad.x(), -grad.y(), 1.0).normalized();
}


/* MAPS */

SF2 HeightField::slope_map() const
{
    SF2 sm(Grid2(*this));

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            sm.at(i, j) = slope(i, j);
        }
    }

    return sm;
}

SF2 HeightField::avg_slope_map() const
{
    SF2 sm(Grid2(*this));

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            sm.at(i, j) = average_slope(i, j);
        }
    }

    return sm;
}

SF2 HeightField::laplacian_map() const
{
    SF2 lm(Grid2(*this));

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            lm.at(i, j) = laplacian(i, j);
        }
    }

    return lm;
}

SF2 HeightField::stream_power() const
{
    SF2 stream = stream_area();
    SF2 slope = slope_map();
    SF2 res(Grid2(*this));
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            res.at(i, j) = sqrt(stream.at(i, j)) * slope.at(i, j);
        }
    }
    return res;
}

SF2 HeightField::wetness_index() const
{
    SF2 stream = stream_area();
    SF2 slope = slope_map();
    SF2 res(Grid2(*this));
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            res.at(i, j) = log(stream.at(i, j)) / (slope.at(i, j) + 0.000001);
        }
    }
    int mean = std::accumulate(field.begin(), field.end(), 0.0) / field.size();
    res.clamp(0.0, mean/10.0f);
    return res;
}

/* STREAM AREA */

const double eps = 0.00001;

std::vector<ScalarPoint2> HeightField::get_scalar_points() const
{
    std::vector<ScalarPoint2> points;
    points.reserve(nx*ny);
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            points.push_back(ScalarPoint2(i, j, height(i, j)));
        }
    }
    assert(int(points.size()) == nx*ny);
    return points;

}

int HeightField::check_flow_slope(const QPoint& p, FlowTiles* ft) const
{
    static const double inv_sqrt_2 = 1.0/sqrt(2.0);
    int n = 0;
    double s[8];

    int i = p.x();
    int j = p.y();
    double zp = height(i, j);

    double slopesum = 0.0;
    for (int ii = -1; ii <= 1; ++ii) {
        for (int jj = -1; jj <= 1; ++jj) {
            int k = i + ii;
            int l = j + jj;
            if (!inside(k, l) || (ii == 0 && jj == 0))
                continue;

            bool diag = ii && jj;
            double step = height(k, l) - zp;
            if (step < -eps) {
                ft->q[n] = QPoint(k, l);
                assert(ft->q[n].x() != i || ft->q[n].y() != j);

                s[n] = -step;
                if(diag)
                    s[n] *= inv_sqrt_2;

                slopesum += s[n];

                ++n;
            }

        }
    }
    assert (n < 9);

    for (int i = 0; i < n; ++i) {
        ft->sn[i] = s[i] / slopesum;
        assert(ft->sn[i] <= 1.0);
    }

    return n;
}

SF2 HeightField::stream_area() const
{
    std::vector<ScalarPoint2> points = get_scalar_points();
    std::sort(points.begin(), points.end());
    for (uint i = 0; i < points.size() - 1; ++i) {
        assert(points[i].z() <= points[i + 1].z());
    }

    SF2 sa = SF2(*this, 1.0);

    for (int i = points.size() - 1; i >= 0; --i)
    {
        const QPoint& p = points[i].point();
        const double sp = sa.at(p);

        FlowTiles ft;
        int n = check_flow_slope(p, &ft);

        if (n == 0)
            continue;

        double checksum = 0.0;
        for (int j = 0; j < n; ++j) {
            checksum += ft.sn[j];
            sa.at(ft.q[j]) += sp*ft.sn[j];
        }
        assert(abs(checksum - 1.0) < eps);
    }

    return sa;

}


/* EXPORT */

QImage HeightField::render(double contrast) const
{
    QImage image(nx, ny, QImage::Format_ARGB32);
    const QVector3D lightdir = QVector3D(2.0, 1.0, 3.0).normalized();

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            QVector3D n = normal(i, j);
            double d = QVector3D::dotProduct(n, lightdir);
            d = (1.0 + d) / 2.0;

            assert(d >= 0.0 && d <= 1.0);
            d = pow(d, contrast);

            d *= 255;
            image.setPixel(i, j, qRgb(d, d, d));
        }
    }

    return image;
}




