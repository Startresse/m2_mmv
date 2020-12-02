#include "heightfield.h"

HeightField::HeightField(const SF2& s) : SF2(s)
{

}

HeightField::HeightField(const QImage& im, const Box2& b, double low, double high)
    : SF2(Grid2(b, im.width(), im.height())), low(low), high(high)
{
    QImage gs = im.convertToFormat(QImage::Format_Grayscale8);
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            at(i, j) = (high - low)*qGray(gs.pixel(i, j))/255.0 + low;
            assert(height(i, j) >= low && height(i, j) <= high);
        }
    }
}


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

const double eps = 0.00001;
int HeightField::check_flow_slope(const QPoint& p, QPoint* q, double* h, double* s, double* sn) const
{
    static const double inv_sqrt_2 = 1.0/sqrt(2.0);
    int n = 0;

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
                q[n] = QPoint(k, l);
                assert(q[n].x() != i || q[n].y() != j);
                h[n] = -step;

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
        sn[i] = s[i] / slopesum;
        assert(sn[i] <= 1.0);
    }

    return n;
}


// steepest
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

        QPoint q[8];		// struct ?
        double h[8];
        double s[8];
        double sn[8];

        int n = check_flow_slope(p, q, h, s, sn);
        if (n == 0)
            continue;

//        double ss = s[0];
//        int k = 0;
        double checksum = 0.0;
        for (int j = 0; j < n; ++j) {
            checksum += sn[j];
            sa.at(q[j]) += sp*sn[j];
//            if (s[j] > ss)
//            {
//                ss = s[j];
//                k = j;
//            }
        }
        assert(abs(checksum - 1.0) < eps);

//        sa.at(q[k].x(), q[k].y()) += sp;
    }

    return sa;

}




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




