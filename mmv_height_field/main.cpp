#include <QApplication>
#include <iostream>

#include "box2.h"
#include "widget.h"

void unitary_tests()
{
    std::cout << "UNITARY TESTS..." << std::endl;

    /* BOX2 */
    {
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
    }

    /* */

    std::cout << "PASSED !" << std::endl;
}

int main(int argc, char *argv[])
{
    // tests
    unitary_tests();
    return 0;

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
