#include <QApplication>
#include <iostream>
#include <QImageReader>

#include "box2.h"
#include "grid2.h"
#include "heightfield.h"
#include "widget.h"

void unitary_tests()
{
    std::cout << "UNITARY TESTS..." << std::endl;

    Box2::test();
    Grid2::test();

    std::cout << "PASSED !" << std::endl;
}

int main(int argc, char *argv[])
{
    // tests
    unitary_tests();
    QImage image;
    image.load("images/test01.png");
    std::cout << image.width() << ", " << image.height() << std::endl;
    image.save("images/modified_image.png");
    return 0;

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
