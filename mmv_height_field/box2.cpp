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


void Box2::test(bool verbose)
{
    if (verbose)
        std::cout << "test box...";
    Box2 b(QVector2D(-1, -1), QVector2D(1, 1));
    Box2 c(QVector2D(1, 1), QVector2D(3, 3));
    assert(b.intersect(c));

    Box2 a(2, 1);
    assert(a.min_x() == -1 && a.max_x() == 1 && a.min_y() == -0.5 && a.max_y() == 0.5);
    assert(a.inside(QVector2D(0, 0)));
    assert(a.inside(QVector2D(1, 0)));
    assert(!a.inside(QVector2D(2, 0)));
    assert(!a.inside(QVector2D(0, 10)));

    Box2 aa(a);
    assert(aa == a);

    Box2 empt;
    assert(empt.max_x() == 0.0 && empt.min_x() == 0.0 && empt.max_y() == 0.0 && empt.min_y() == 0.0);

    if (verbose)
        std::cout << " Done!" << std::endl;

}
