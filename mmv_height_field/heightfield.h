#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <QVector3D>
#include <QImage>

#include "sf2.h"

class HeightField : public SF2
{
public:
    HeightField();

    double Height(int i, int j) const;
    double Slope(int i, int j) const;

    double AverageSlope(int i, int j) const;

    QVector3D p_3d(int i, int j) const;
    QVector3D n_3d(int i, int j) const;

    void Shade() const;

    QImage Export() const;

    SF2 StreamArea() const;
};

#endif // HEIGHTFIELD_H
