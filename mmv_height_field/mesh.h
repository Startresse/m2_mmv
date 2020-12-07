#pragma once

#include <QGLWidget>
#include "heightfield.h"

class PointIJ : public QVector3D
{
public:
    int i;
    int j;

    PointIJ() : QVector3D(), i(-1), j(-1) {}
    PointIJ(QVector3D _p) : PointIJ(_p, -1, -1) {}
    PointIJ(QVector3D _p, int _i, int _j) : QVector3D(_p), i(_i), j(_j) {}
    PointIJ(double _x, double _y, double _z, int _i, int _j) : PointIJ(QVector3D(_x, _y, _z), _i, _j){}
};

class Triangles
{
public:
    PointIJ vertices[3];

    Triangles(const PointIJ& a, const PointIJ& b, const PointIJ& c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }
};


enum render_type {RENDER, STREAM_AREA};

// MESH
class Mesh {
private:
    HeightField hf;
    std::vector<Triangles> faces;
    QImage render;

    SF2 rendersf;
    double max;

public:
    Mesh() {
//        QImage image;
//        image.load("images/vesuve.png");
//        hf = HeightField(image, Box2(15000, 15000), 0.0, 1200.0);
        QImage image;
        image.load("images/heightmap3.jpeg");
        hf = HeightField(image, Box2(3000, 3000), 0.0, 1200.0);
        render = hf.render(3.0);
        rendersf = hf.stream_area();
        max = rendersf.max();
    }

    ~Mesh() {}

    void set_up();
    void change_render(int, double pow = 1.0);

    void glTriangle(const Triangles& t);
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

    Mesh& get_mesh() {return _mesh;}

};
