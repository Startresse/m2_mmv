#ifndef SCALARPOINT2_H
#define SCALARPOINT2_H

#include <QPoint>

class ScalarPoint2
{
protected:
    QPoint p;
    double h;

public:
    ScalarPoint2();
    ScalarPoint2(int, int, double);

    friend bool operator<(const ScalarPoint2&, const ScalarPoint2&);

    QPoint point() {return p;}
    double z() {return h;}
};

#endif // SCALARPOINT2_H
