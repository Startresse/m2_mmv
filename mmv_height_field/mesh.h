#pragma once

#include <QGLWidget>
#include <QMap>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <stack>
#include <vector>



// MESH

class Mesh {
private:
public:
    Mesh() {}
    ~Mesh() {}
    void draw();

};

// GEOMETRY WORLD

class GeometricWorld {
private:
    Mesh _mesh;

public :
    GeometricWorld() {
    }

    void draw() {
        _mesh.draw();
    }

};
