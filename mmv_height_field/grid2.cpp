#include "grid2.h"

Grid2::Grid2(const Box2& b, int nx, int ny) : Box2(b), nx(nx), ny(ny)
{
    diag = b_max - b_min;
    cell_diag = QVector2D(diag.x() / (nx - 1), diag.y() / (ny - 1));
    inv_cell_diag = QVector2D(1.0/cell_diag.x(), 1.0/cell_diag.y());
}

Grid2::Grid2(const Grid2& g) : Grid2(static_cast<Box2>(g), g.nx, g.ny)
{

}

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

void Grid2::test(bool verbose)
{
    if (verbose)
        std::cout << "test grid...";

    Box2 b(50, 100);
    Grid2 a(b, 10, 10);
    assert(a.index(1, 1) == 11 && a.index(0, 0) == 0 && a.index(9, 9) == 99);
    assert(!a.inside(10,0) && a.inside(0, 0));
    assert(a.border(0, 2) && !a.border(3, 2));

    Grid2 aa(a);
    assert(a == aa);

    if (verbose)
        std::cout << " Done!" << std::endl;
}
