#include "sf2.h"

SF2::SF2()
{

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

    if (!border(i, j)) {
        g.setX(at(i + 1, j) - at(i - 1, j)); // coord inverse diag + border !!
        g.setY(at(i, j + 1) - at(i, j - 1)); // * 0.5 * inve diag
    }

    return g;

    //df/dx,df/dy ~ ( (f(x+e,y)-f(x-e,y))/2e , ... ) { Vec2 n; // Gradient along x axis if (i == 0) { n[0] = (at(i + 1, j) - at(i, j)) * inversecelldiagonal[0]; } else if (i == nx - 1) { n[0] = (at(i, j) - at(i - 1, j)) * inversecelldiagonal[0]; } else { n[0] = (at(i + 1, j) - at(i - 1, j)) *0.5 * inversecelldiagonal[0]; } // Gradient along y axis if (j == 0) { n[1] = (at(i, j + 1) - at(i, j)) * inversecelldiagonal[1]; } else if (j == ny - 1) { n[1] = (at(i, j) - at(i, j - 1)) * inversecelldiagonal[1]; } else { n[1] = (at(i, j + 1) - at(i, j - 1)) * 0.5 * inversecelldiagonal[1]; } return n; }

}

// d2f / dx2 ~ (f(x+e)-2f(x)+f(x+e))/(e^2) { double laplacian = 0.0; // Divergence along x axis if (i == 0) { laplacian += (at(i, j) - 2.0 * at(i + 1, j) + at(i + 2, j)) / (celldiagonal[0] * celldiagonal[0]); } else if (i == nx - 1) { laplacian += (at(i, j) - 2.0 * at(i - 1, j) + at(i - 2, j)) / (celldiagonal[0] * celldiagonal[0]); } else { laplacian += (at(i + 1, j) - 2.0 * at(i, j) + at(i - 1, j)) / (celldiagonal[0] * celldiagonal[0]); } // Divergence along y axis if (j == 0) { laplacian += (at(i, j) - 2.0 * at(i, j + 1) + at(i, j + 2)) / (celldiagonal[1] * celldiagonal[1]); } else if (j == ny - 1) { laplacian += (at(i, j) - 2.0 * at(i, j - 1) + at(i, j - 2)) / (celldiagonal[1] * celldiagonal[1]); } else { laplacian += (at(i, j + 1) - 2.0 * at(i, j) + at(i, j - 1)) / (celldiagonal[1] * celldiagonal[1]); } return laplacian; }
