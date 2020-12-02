#include <QApplication>
#include <iostream>
#include <QImageReader>

#include "box2.h"
#include "grid2.h"
#include "heightfield.h"
#include "widget.h"

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
    image.load("../images/mb_height.png");

    HeightField hf(image, Box2(image.width(), image.height()), 10.0, 200.0);
    hf.render(3.0).save("../images/relief.png");
//    QImage res = hf.render(3.0);

//    QImage res = hf.slope_map().save();
//    QImage res = hf.laplacian_map().save();
    QImage res = hf.stream_area().save(0.03);

    res.save("../images/res.png");
    return 0;

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
