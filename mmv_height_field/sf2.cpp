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


void SF2::clamp(double min, double max)
{
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            at(i, j) = std::min(std::max(min, at(i, j)), max);
        }
    }
}

SF2 SF2::normalize(bool& positive) const
{
    SF2 ret(*this);

    auto [minp, maxp] = std::minmax_element(begin(field), end(field));
    height_t min = *minp;
    height_t max = *maxp;

    assert(max > 0);
    positive = min >= 0.0;
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            if (positive) {
                ret.at(i, j) = (at(i, j) - min)/(max - min);
            } else {
                ret.at(i, j) = (at(i, j) / (std::max(max, -min)));
            }
        }
    }

    return ret;
}


const double eps = 0.00001;
// TODO divergente palette
QImage SF2::save(double contrast) const
{
    QImage image(nx, ny, QImage::Format_ARGB32);

    bool positive;
    SF2 sf_norm = normalize(positive);

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            float val = sf_norm.at(i, j);

            assert(val >= positive ? - eps : -1 - eps && val <= 1.0 + eps);

            if (val > 0)
                val = pow(val, contrast);
            else
                val = -pow(-val, contrast);

            QRgb color;
            if (positive) {
                double nv = img_max_value * val;
                color = qRgb(nv, nv, nv);
            } else {
                double def = img_max_value;
                if (val > 0)
                    color = qRgb(def, (1 - val) * def, (1 - val) * def);
                else
                    color = qRgb((1 + val) * def, (1 + val) * def, def);
            }
            image.setPixel(i, j, color);
        }
    }

    return image;
}

const double gauss3_fact = 1.0/16.0;
const int gauss3[3][3] = {{1, 2, 1},{2, 4, 2},{1, 2, 1}};

const double gauss5_fact = 1.0/256.0;
const int gauss5[5][5] = {
    {1,  4,  6,  4, 1},
    {4, 16, 24, 16, 4},
    {6, 24, 36, 24, 6},
    {4, 16, 24, 16, 4},
    {1,  4,  6,  4, 1}
};

void SF2::blur(int n)
{
    if (n < 1)
        return;
    if (n > 2)
        n = 2;

    SF2 res = SF2(*this);
    for (int i = n; i < nx - n; ++i) {
        for (int j = n; j < ny - n; ++j) {
            double sum = 0;
            for (int k = -n; k <= n; ++k) {
                for (int l = -n; l <= n; ++l) {
                    if (n == 1)
                        sum += res.at(i + k, j + l)*gauss3[k + n][l + n];
                    else
                        sum += res.at(i + k, j + l)*gauss5[k + n][l + n];
                }
            }
            if (n == 1)
                at(i, j) = sum * gauss3_fact;
            else
                at(i, j) = sum * gauss5_fact;

        }
    }
}

void SF2::median(int n)
{
    if (n <= 0)
        return;

    SF2 res = SF2(*this);
    for (int i = n; i < nx - n; ++i) {
        for (int j = n; j < ny - n; ++j) {

            double sum = 0;
            for (int k = -n; k <= n; ++k) {
                for (int l = -n; l <= n; ++l) {
                    sum += res.at(i + k, j + l);
                }
            }
            at(i, j) = sum / ((2*n + 1) * (2*n + 1));

        }
    }
}


