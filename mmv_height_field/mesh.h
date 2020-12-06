#pragma once

#include <QGLWidget>
#include "heightfield.h"


class Triangles
{
public:
    QVector3D vertices[3];

    Triangles(QVector3D a, QVector3D b, QVector3D c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }
};

// MESH
class Mesh {
private:
    HeightField hf;
    std::vector<Triangles> faces;

public:
    Mesh() {
        QImage image;
        image.load("images/vesuve.png");
        hf = HeightField(image, Box2(15000, 15000), 0.0, 1200.0);
    }

    ~Mesh() {}

    void set_up();

    void draw();

};

// GEOMETRY WORLD

class GeometricWorld {
private:
    Mesh _mesh;

public :
    GeometricWorld()
    {
        _mesh.set_up();
    }

    void draw()
    {
        _mesh.draw();
    }

};
