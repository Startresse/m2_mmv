#include "mesh.h"


void Mesh::set_up()
{
    // BOTTOM
    faces.emplace_back(QVector3D(-2, 0, -2),
                       QVector3D(-2, 0,  2),
                       QVector3D( 2, 0,  2));

    faces.emplace_back(QVector3D(-2, 0, -2),
                       QVector3D( 2, 0, -2),
                       QVector3D( 2, 0,  2));


    float length_x = hf.size_x()  / 4;
    float length_y = hf.highest();
    float length_z = hf.size_y()  / 4;

    // FRONT & BACK
    std::vector<int> borders = {0, hf.size_y() - 1};
    for (int j : borders) {
        for (int i = 0; i < hf.size_x() - 1; ++i) {
            faces.emplace_back( QVector3D((i    )/length_x - 2, hf.height(i    , j) / length_y, -j/length_z + 2),
                                QVector3D((i    )/length_x - 2, 0                             , -j/length_z + 2),
                                QVector3D((i + 1)/length_x - 2, 0                             , -j/length_z + 2));

            faces.emplace_back( QVector3D((i    )/length_x - 2, hf.height(i    , j) / length_y, -j/length_z + 2),
                                QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j) / length_y, -j/length_z + 2),
                                QVector3D((i + 1)/length_x - 2, 0                             , -j/length_z + 2));
        }
    }

    // SIDES
    borders = {0, hf.size_x() - 1};
    for (int j : borders) {
        for (int i = 0; i < hf.size_y() - 1; ++i) {
            faces.emplace_back( QVector3D(j/length_x - 2, hf.height(j, i    ) / length_y, -(i    )/length_z + 2),
                                QVector3D(j/length_x - 2, 0                             , -(i    )/length_z + 2),
                                QVector3D(j/length_x - 2, 0                             , -(i + 1)/length_z + 2));

            faces.emplace_back( QVector3D(j/length_x - 2, hf.height(j, i    ) / length_y, -(i    )/length_z + 2),
                                QVector3D(j/length_x - 2, hf.height(j, i + 1) / length_y, -(i + 1)/length_z + 2),
                                QVector3D(j/length_x - 2, 0                             , -(i + 1)/length_z + 2));
        }
    }

    // HF
    for (int i = 0; i < hf.size_x() - 1; ++i) {
        for (int j = 0; j < hf.size_y() - 1; ++j) {
            faces.emplace_back( QVector3D((i    )/length_x - 2, hf.height(i    , j    ) / length_y, -(j    )/length_z + 2),
                                QVector3D((i    )/length_x - 2, hf.height(i    , j + 1) / length_y, -(j + 1)/length_z + 2),
                                QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j + 1) / length_y, -(j + 1)/length_z + 2));

            faces.emplace_back( QVector3D((i    )/length_x - 2, hf.height(i    , j    ) / length_y, -(j    )/length_z + 2),
                                QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j    ) / length_y, -(j    )/length_z + 2),
                                QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j + 1) / length_y, -(j + 1)/length_z + 2));
        }
    }

    std::cout << faces.size() << std::endl;

}


inline
void glPoint(const QVector3D& v)
{
    glVertex3d(v.x(), v.y(), v.z());
}

inline
void glTriangle(const Triangles& t)
{
    for (int i = 0; i < 3; ++i) {
        glPoint(t.vertices[i]);
    }
}

void Mesh::draw() {
    srand(0);
    glColor3d(.8, .8, .8);

    glBegin(GL_TRIANGLES);
    for (const Triangles& t : faces)
        glTriangle(t);
    glEnd();
}
