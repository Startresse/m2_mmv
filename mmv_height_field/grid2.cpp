#include "grid2.h"

Grid2::Grid2(const Box2& b, int nx, int ny) : Box2(b), nx(nx), ny(ny) {}
Grid2::Grid2(const Grid2& g) : Grid2(static_cast<Box2>(g), g.nx, g.ny) {}

int Grid2::index(int i, int j) const
{
    return i + j * nx;
}

bool Grid2::inside(int i, int j) const
{
    return i >= 0 && j >= 0 && i < nx && j < ny;
}

bool Grid2::border(int i, int j) const
{
    return i == 0 || j == 0 || i == nx - 1 || j == ny - 1;
}

QVector2D Grid2::vertex(int i, int j) const
{
    double u = double(i) / (nx - 1);
    float x = (1 - u) * b_min.x() + u * b_max.x();
    double v = double(j) / (ny - 1);
    float y = (1 - v) * b_min.y() + v * b_max.y();
    return QVector2D(x, y);
}


bool Grid2::operator==(const Grid2& b) const
{
    return static_cast<Box2>(*this) == static_cast<Box2>(b) && nx == b.nx && ny == b.ny;
}
