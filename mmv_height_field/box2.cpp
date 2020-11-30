#include "box2.h"

Box2::Box2()
{
    b_min = QVector2D(0, 0);
    b_max = QVector2D(0, 0);
}

Box2::Box2(const QVector2D& min, const QVector2D& max)
{
    b_min = min;
    b_max = max;
}

Box2::Box2(double x, double y) {
    b_min = QVector2D(-x/2.0, -y/2.0);
    b_max = QVector2D( x/2.0,  y/2.0);
}

Box2::Box2(const Box2& b) {
    b_min = b.b_min;
    b_max = b.b_max;
}


bool Box2::inside(const QVector2D& p) const
{
    return !(
        p.x() > b_max.x() ||
        p.y() > b_max.y() ||
        p.x() < b_min.x() ||
        p.y() < b_min.y());
}

bool Box2::intersect(const Box2& b) const
{
    return !(
        b.b_min.x() > b_max.x() ||
        b.b_min.y() > b_max.y() ||
        b.b_max.x() < b_min.x() ||
        b.b_max.y() < b_min.y());
}

bool Box2::operator==(const Box2& b) const
{
    return b.b_max == b_max && b.b_min == b_min;
}
