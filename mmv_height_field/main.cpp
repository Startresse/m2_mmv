#include <QApplication>
#include <iostream>

#include "box2.h"
#include "grid2.h"
#include "heightfield.h"
#include "widget.h"
#include "mainwindow.h"

void unitary_tests(bool verbose = false)
{
    if (verbose)
        std::cout << "UNITARY TESTS..." << std::endl;

    Box2::test(verbose);
    Grid2::test(verbose);

    if (verbose)
        std::cout << "PASSED !" << std::endl;
}

int main(int argc, char *argv[])
{
    // tests
    unitary_tests(false);
    QImage image;
    image.load("../images/vesuve.png");

    HeightField hf(image, Box2(15000, 15000), 0.0, 1200.0);
//    QImage res = hf.render(3.0);

//    QImage res = hf.slope_map().save();
//    QImage res = hf.laplacian_map().save();
    QImage res = hf.stream_area().save(0.5);






    hf.render(3.0).save("../images/render.png");
    res.save("../images/res.png");
//    return 0;

    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    MainWindow w;
    w.show();


    std::cout << "OK" << std::endl;
//    return 0;
    return a.exec();
}
