#include "mesh.h"


void Mesh::draw() {
    glColor3d(.8, .8, .8);

    glBegin(GL_TRIANGLES);
    glVertex3f(1, 0 ,0);
    glVertex3f(1, 1 ,0);
    glVertex3f(0, 0 ,0);

    glEnd();
}
