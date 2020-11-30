#ifndef BOX2_H
#define BOX2_H

#include <QVector2D>

class Box2
{
protected:
    QVector2D b_min;
    QVector2D b_max;

public:
    Box2();
    Box2(float x, float y);
    Box2(const QVector2D& min, const QVector2D& max);
    Box2(const Box2& b);

    bool inside(const QVector2D&) ;
    bool intersect(const Box2&) ;

    double min_x() {return b_min.x();}
    double min_y() {return b_min.y();}
    double max_x() {return b_max.x();}
    double max_y() {return b_max.y();}

    bool operator==(const Box2&);
};

#endif // BOX2_H
