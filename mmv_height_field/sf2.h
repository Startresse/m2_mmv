#ifndef SF2_H
#define SF2_H

#include <QVector>
#include <QImage>
#include <math.h>

#include "grid2.h"

typedef double height_t ;
class SF2 : public Grid2
{
protected:
    std::vector<height_t> field;

//    height_t min = inf;
//    height_t max = -inf;

public:
    SF2();
    SF2(const Grid2& g);
    SF2(const Grid2& g, double);

    double at(int i, int j) const;
    double& at(int i, int j);
    double at(const QPoint&) const;
    double& at(const QPoint&);

    QVector2D Gradient(int i, int j) const;
    double laplacian(int i, int j) const;

    QImage save(double contrast = 1.0) const;
};

#endif // SF2_H
