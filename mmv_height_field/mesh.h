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


enum render_type {
    RENDER,
    STREAM_AREA,
    STREAM_POWER,
    LAPLACIAN,
    SLOPE,
    AVG_SLOPE,
    WETNESS,
    ROAD,
};

enum images {
    SPYKES,
    CHAM,
    MB,
    VESUVE,
    VOLC,
};

// MESH
// TODO : inherit form hf ?
class Mesh {
private:
    std::vector<Triangles> faces;

    HeightField hf;
    QImage render_img;
    QImage render_tex;

    bool shading_done;
    QImage shading;

    double blend_fact = 0.5;

    bool closed = false;
    bool fill_road = true;

    QString image_name = "images/heightmap3.jpeg";
    double length_x = 2000;
    double length_y = 2000;
    double low = 0;
    double high = 1200;

    render_type render_t = RENDER;
    double render_power = 3.0;

    int blur_strength = 1.0;
    bool blur_no_median = true;

    QPoint path_begin;
    QPoint path_end;

public:
    Mesh() {
        load();
    }

    ~Mesh() {}


    void load();
    void set_up();

    void update_file(int);

    void blur();
    void update_blur_strength(int);
    void update_blur_type(bool);

    void render();
    void update_render_type(int);
    void update_render_power(double);

    void update_tex_blend();
    void update_blend(double t);

    QImage display_road();
    void update_path(const QPoint& a, const QPoint& b);

    void glTriangle(const Triangles& t);
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

    Mesh& get_mesh() {return _mesh;}

};
