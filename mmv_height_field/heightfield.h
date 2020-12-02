#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <QVector3D>
#include <QImage>
#include <math.h>
#include <QRgb>

#include "scalarpoint2.h"
#include "sf2.h"

class HeightField : public SF2
{
protected:
    double low;
    double high;

public:
    HeightField(const SF2&);
    HeightField(const QImage&, const Box2&, double, double);

    double height(int i, int j) const;
    double slope(int i, int j) const;
    double average_slope(int i, int j) const;

    QVector3D vertex(int i, int j) const;
    QVector3D normal(int i, int j) const;

    std::vector<ScalarPoint2> get_scalar_points() const;
    int check_flow_slope(const QPoint&, QPoint*, double*, double*) const;

    SF2 slope_map() const;
    SF2 laplacian_map() const;
    SF2 stream_area() const;

    void Shade() const;

    QImage render(double contrast = 1.0) const;

};

#endif // HEIGHTFIELD_H
