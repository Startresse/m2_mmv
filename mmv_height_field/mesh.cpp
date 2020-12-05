#include "mesh.h"


void glRandomColor() {
    glColor3d((float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX);
}

void Mesh::draw() {
    srand(0);
    glColor3d(.8, .8, .8);

    float length_x = hf.size_x()  / 4;
    float length_y = hf.highest() / 4;
    float length_z = hf.size_y()  / 4;

    glBegin(GL_TRIANGLES);
    // BOTTOM
    glVertex3d(-2, 0, -2);
    glVertex3d(-2, 0, 2);
    glVertex3d(2, 0, 2);

    glVertex3d(-2, 0, -2);
    glVertex3d(2, 0, -2);
    glVertex3d(2, 0, 2);
    // FRONT
    std::vector<int> borders = {0, hf.size_y() - 1};
    for (int j : borders) {
        for (int i = 0; i < hf.size_x() - 1; ++i) {
            glVertex3d((i    )/length_x - 2, hf.height(i    , j) / length_y, -j/length_z + 2);
            glVertex3d((i    )/length_x - 2, 0                             , -j/length_z + 2);
            glVertex3d((i + 1)/length_x - 2, 0                             , -j/length_z + 2);

            glVertex3d((i    )/length_x - 2, hf.height(i    , j) / length_y, -j/length_z + 2);
            glVertex3d((i + 1)/length_x - 2, hf.height(i + 1, j) / length_y, -j/length_z + 2);
            glVertex3d((i + 1)/length_x - 2, 0                             , -j/length_z + 2);
        }
    }
    // SIDES
    borders = {0, hf.size_x() - 1};
    for (int j : borders) {
        for (int i = 0; i < hf.size_y() - 1; ++i) {
            glVertex3d(j/length_x - 2, hf.height(j, i    ) / length_y, -(i    )/length_z + 2);
            glVertex3d(j/length_x - 2, 0                             , -(i    )/length_z + 2);
            glVertex3d(j/length_x - 2, 0                             , -(i + 1)/length_z + 2);

            glVertex3d(j/length_x - 2, hf.height(j, i    ) / length_y, -(i    )/length_z + 2);
            glVertex3d(j/length_x - 2, hf.height(j, i + 1) / length_y, -(i + 1)/length_z + 2);
            glVertex3d(j/length_x - 2, 0                             , -(i + 1)/length_z + 2);
        }
    }

    for (int i = 0; i < hf.size_x() - 1; ++i) {
        for (int j = 0; j < hf.size_y() - 1; ++j) {
            glVertex3d((i    )/length_x - 2, hf.height(i    , j    ) / length_y, -(j    )/length_z + 2);
            glVertex3d((i    )/length_x - 2, hf.height(i    , j + 1) / length_y, -(j + 1)/length_z + 2);
            glVertex3d((i + 1)/length_x - 2, hf.height(i + 1, j + 1) / length_y, -(j + 1)/length_z + 2);

            glVertex3d((i    )/length_x - 2, hf.height(i    , j    ) / length_y, -(j    )/length_z + 2);
            glVertex3d((i + 1)/length_x - 2, hf.height(i + 1, j    ) / length_y, -(j    )/length_z + 2);
            glVertex3d((i + 1)/length_x - 2, hf.height(i + 1, j + 1) / length_y, -(j + 1)/length_z + 2);
        }
    }

    glEnd();
}
