#include "sf2.h"

SF2::SF2() : SF2(Grid2(), 0.0) { }

SF2::SF2(const Grid2& g) : SF2(g, 0.0) { }

SF2::SF2(const Grid2& g, double val) : Grid2(g)
{
    field.resize(nx * ny, val);
}

SF2::SF2(const SF2& sf) : SF2(static_cast<Grid2>(sf))
{
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            at(i, j) = sf.at(i, j);
        }
    }
}

double SF2::at(int i, int j) const
{
    return field[index(i, j)];
}
double& SF2::at(int i, int j)
{
    return field[index(i, j)];
}

double SF2::at(const QPoint& p) const {return at(p.x(), p.y());}
double& SF2::at(const QPoint& p) {return at(p.x(), p.y());}


QVector2D SF2::Gradient(int i, int j) const
{
    QVector2D g;

    if (i == 0)
        g.setX( (at(i + 1, j) - at(i, j)    )       * inv_cell_diag.x() );
    else if (i == nx - 1)
        g.setX( (at(i, j) 	  - at(i - 1, j)) 	    * inv_cell_diag.x() );
    else
        g.setX( (at(i + 1, j) - at(i - 1, j)) * 0.5 * inv_cell_diag.x() );

    if (j == 0)
        g.setY( (at(i, j + 1) - at(i, j)    )       * inv_cell_diag.y() );
    else if (j == ny - 1)
        g.setY( (at(i, j) 	  - at(i, j - 1)) 	    * inv_cell_diag.y() );
    else
        g.setY( (at(i, j + 1) - at(i, j - 1)) * 0.5 * inv_cell_diag.y() );


    return g;
}

double SF2::laplacian(int i, int j) const
{
    double lap = 0.0;

    if (i == 0)
        lap += (at(i	, j) - 2.0 * at(i + 1, j) + at(i + 2, j)) * inv_cell_diag.x() * inv_cell_diag.x();
    else if (i == nx - 1)
        lap += (at(i - 2, j) - 2.0 * at(i - 1, j) + at(i	, j)) * inv_cell_diag.x() * inv_cell_diag.x();
    else
        lap += (at(i - 1, j) - 2.0 * at(i	 , j) + at(i + 1, j)) * inv_cell_diag.x() * inv_cell_diag.x();

    if (j == 0)
        lap += (at(i, j    ) - 2.0 * at(i, j + 1) + at(i, j + 2)) * inv_cell_diag.y() * inv_cell_diag.y();
    else if (j == ny - 1)
        lap += (at(i, j - 2) - 2.0 * at(i, j - 1) + at(i, j	   )) * inv_cell_diag.y() * inv_cell_diag.y();
    else
        lap += (at(i, j - 1) - 2.0 * at(i, j	) + at(i, j + 1)) * inv_cell_diag.y() * inv_cell_diag.y();

    return lap;
}

double SF2::max()
{
    double max = at(0, 0);
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            double val = at(i, j);
            if(val > max)
                max = val;
        }
    }
    auto smax = std::max_element(field.begin(), field.end());
    assert(*smax == max);

    return max;
}

const double eps = 0.00001;
QImage SF2::save(double contrast) const
{
    QImage image(nx, ny, QImage::Format_ARGB32);

    double max = at(0, 0);
    double min = at(0, 0);
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            double val = at(i, j);
            if(val > max)
                max = val;
            if(val < min)
                min = val;
        }
    }

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            if (abs(min - max) <= eps) {
                image.setPixel(i, j, qRgb(125, 125, 125));
                continue;
            }
            float val = at(i, j);
            val = (val - min)/(max - min);

            if (contrast != 1.0)
                assert(val >= -eps && val <= 1.0 + eps);

            val = pow(val, contrast);
            val *= img_max_value;
            image.setPixel(i, j, qRgb(val, val, val));
        }
    }

    return image;
}

void SF2::soften(int n)
{
    SF2 res = SF2(*this);
    for (int i = n; i < nx - n; ++i) {
        for (int j = n; j < ny - n; ++j) {
            double sum = 0;
            for (int k = -n; k <= n; ++k) {
                for (int l = -n; l <= n; ++l)
                    sum += res.at(i + k, j + l);
            }
            at(i, j) = sum / ((2*n + 1) * (2*n + 1));

        }
    }

}


