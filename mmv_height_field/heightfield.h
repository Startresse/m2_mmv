#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <QVector3D>
#include <QImage>
#include <math.h>
#include <QRgb>
#include <opencv2/core.hpp>

#include "scalarpoint2.h"
#include "sf2.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"


class HeightField : public SF2
{
protected:
    double low;
    double high;

public:
    HeightField() : SF2() {};
    HeightField(const cv::Mat&, const Box2&, double, double);
    HeightField(const HeightField&);

    // properties
    double height(int i, int j) const;
    double slope(int i, int j) const;
    double average_slope(int i, int j) const;

    QVector3D vertex(int i, int j) const;
    QVector3D normal(int i, int j) const;

    // stream area
    struct FlowTiles {
        QPoint q[8];
        double sn[8];
    };
    std::vector<ScalarPoint2> get_scalar_points() const;
    int check_flow_slope(const QPoint&, FlowTiles*) const;


    // maps
    SF2 slope_map() const;
    SF2 avg_slope_map() const;
    SF2 laplacian_map() const;
    SF2 stream_area() const;
    SF2 stream_power() const;
    SF2 wetness_index() const;

    // export
    QImage render(double contrast = 1.0) const;

    // getter
    int size_x() const {return nx;}
    int size_y() const {return ny;}
    int highest() const {return high;}
    int lowest() const {return low;}

};

#endif // HEIGHTFIELD_H
