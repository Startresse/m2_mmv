#include "sf2.h"

SF2::SF2() : SF2(Grid2(), 0.0) { }

SF2::SF2(const Grid2& g) : SF2(g, 0.0) { }

SF2::SF2(const Grid2& g, double val) : Grid2(g)
{
    field.resize(nx * ny, val);
}

double SF2::at(int i, int j) const
{
    return field[index(i, j)];
}
double& SF2::at(int i, int j)
{
    return field[index(i, j)];
}

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
