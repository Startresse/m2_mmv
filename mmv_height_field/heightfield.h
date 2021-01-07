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

//#define DISPLAY_TIME

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>
typedef int vertex_t;
typedef double weight_t;
const weight_t max_weight = std::numeric_limits<weight_t>::infinity();
struct neighbor
{
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) {}
};

typedef std::vector<std::vector<neighbor>> adjacency_list_t;

std::ostream& operator<<(std::ostream& os, const QPoint& p);


class HeightField : public SF2
{
protected:
    double low;
    double high;

    adjacency_list_t adjacency_list;
    bool list_built = false;

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
    bool water(const QPoint& a) {return height(a.x(), a.y()) <= low + 0.0001;}


    // Dijkstra related

    double weight(const QPoint& a, const QPoint& b);
    void build_adjacency_list();
    std::list<vertex_t> shortest_path(const QPoint& a, const QPoint& b);


};

#endif // HEIGHTFIELD_H
