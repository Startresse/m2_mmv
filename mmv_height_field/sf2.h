#ifndef SF2_H
#define SF2_H

#include <QVector>

#include "grid2.h"

typedef double height_t ;
class SF2 : public Grid2
{
protected:
    std::vector<height_t> field;

public:
    SF2();
    SF2(const Grid2& g);
    SF2(const Grid2& g, double);

    double at(int i, int j) const;
    double& at(int i, int j);

    QVector2D Gradient(int i, int j) const;
    double laplacian(int i, int j) const;

    void save() const;
};

#endif // SF2_H
