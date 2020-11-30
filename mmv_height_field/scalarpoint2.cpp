#include "scalarpoint2.h"

ScalarPoint2::ScalarPoint2()
{

}

ScalarPoint2::ScalarPoint2(int i, int j, double z) : p(QPoint(i, j)), h(z)
{

}


bool operator<(const ScalarPoint2& a, const ScalarPoint2& b) {
    return a.h < b.h;
}
