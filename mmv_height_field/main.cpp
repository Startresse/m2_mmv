#include <iostream>

#include "widget.h"
#include "box2.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
//    w.show();

    /* TESTS */

    Box2 b(QVector2D(-1, -1), QVector2D(1, 1));
    Box2 c(QVector2D(1, 1), QVector2D(3, 3));
    std::cout << b.intersect(c) << std::endl;




    return 0;

    /* END TESTS */

    return a.exec();
}
