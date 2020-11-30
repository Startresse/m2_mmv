#ifndef BOX2_H
#define BOX2_H

#include <iostream>
#include <QVector2D>

class Box2
{
protected:
    QVector2D b_min;
    QVector2D b_max;

public:
    Box2();
    Box2(double x, double y);
    Box2(const QVector2D& min, const QVector2D& max);
    Box2(const Box2& b);

    bool inside(const QVector2D&) const ;
    bool intersect(const Box2&) const ;

    double min_x() const {return b_min.x();}
    double min_y() const {return b_min.y();}
    double max_x() const {return b_max.x();}
    double max_y() const {return b_max.y();}

    bool operator==(const Box2&) const;

    static void test();
};

#endif // BOX2_H
