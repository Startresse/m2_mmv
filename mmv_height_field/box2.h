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
};

#endif // BOX2_H
