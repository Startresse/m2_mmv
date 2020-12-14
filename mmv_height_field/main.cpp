#include <QApplication>
#include <iostream>

#include "box2.h"
#include "grid2.h"
#include "heightfield.h"
#include "mainwindow.h"

#include "opencv2/core.hpp"

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

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
