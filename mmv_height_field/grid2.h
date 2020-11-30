#ifndef GRID2_H
#define GRID2_H

#include "box2.h"

class Grid2 : public Box2
{
protected:
    int nx;
    int ny;

public:
    Grid2(const Box2& b = Box2(), int nx = 0, int ny = 0);
    Grid2(const Grid2& g);

    int index(int i, int j) const;
    bool inside(int i, int j) const;
    bool border(int i, int j) const;

    QVector2D vertex(int i, int j) const;

    bool operator==(const Grid2&) const;
};

#endif // GRID2_H
