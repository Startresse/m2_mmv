#pragma once

#include <QGLWidget>
#include "heightfield.h"

// MESH

class Mesh {
private:
    HeightField hf;

public:
    Mesh() {
        QImage image;
        image.load("images/cham.png");
        hf = HeightField(image, Box2(15000, 15000), 0.0, 1200.0);
    }

    ~Mesh() {}
    void draw();

};

// GEOMETRY WORLD

class GeometricWorld {
private:
    Mesh _mesh;

public :
    GeometricWorld() {}

    void draw()
    {
        _mesh.draw();
    }

};
