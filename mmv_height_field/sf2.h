#ifndef SF2_H
#define SF2_H

#include <QVector>
#include <QImage>
#include <math.h>

#include "grid2.h"

constexpr double img_max_value = 255.0;

typedef double height_t ;
class SF2 : public Grid2
{
protected:
    std::vector<height_t> field;

public:
    SF2();
    SF2(const Grid2& g);
    SF2(const Grid2& g, double);
    SF2(const SF2&);

    double at(int i, int j) const;
    double& at(int i, int j);
    double at(const QPoint&) const;
    double& at(const QPoint&);

    QVector2D Gradient(int i, int j) const;
    double laplacian(int i, int j) const;

    void median(int n = 1);
    void blur(int n = 1);

    void clamp(double min, double max);
    SF2 normalize(bool& positive) const;

    QImage save(double contrast = 1.0) const;

};

#endif // SF2_H
