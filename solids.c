#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// Constants for angles and translations
const float size = 1.0; // Size of the solids

// Rotation angles for each solid
float rotation_angles[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
int selected_solid = -1; // No solid selected initially

// Function to initialize open_gl
void init_opengl() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

// Function to draw a tetrahedron
void draw_tetrahedron() {
    glLoadName(0);
    glutSolidTetrahedron();
}

// Function to draw a cube
void draw_cube() {
    glLoadName(1);
    glutSolidCube(size);
}

// Function to draw an octahedron
void draw_octahedron() {
    glLoadName(2);
    glutSolidOctahedron();
}

// Function to draw an icosahedron
void draw_icosahedron() {
    glLoadName(3);
    glutSolidIcosahedron();
}

// Function to draw a dodecahedron
void draw_dodecahedron() {
    glLoadName(4);
    glutSolidDodecahedron();
}

// Function to draw all Platonic solids
void draw_solids() {
    glPushMatrix();
    glTranslatef(-3.0, 2.0, -10.0);
    glRotatef(rotation_angles[0], 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0); // Red
    draw_tetrahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 2.0, -10.0);
    glRotatef(rotation_angles[1], 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0); // Green
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.0, 2.0, -10.0);
    glRotatef(rotation_angles[2], 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0); // Blue
    draw_octahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, -2.0, -10.0);
    glRotatef(rotation_angles[3], 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 0.0); // Yellow
    draw_icosahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, -2.0, -10.0);
    glRotatef(rotation_angles[4], 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 1.0); // Magenta
    draw_dodecahedron();
    glPopMatrix();
}

// Display function to render the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    draw_solids();

    glutSwapBuffers();
}

// Reshape function to handle window resizing
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void pick_solid(int x, int y) {
    GLuint select_buf[512];
    GLint hits, viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    glSelectBuffer(512, select_buf);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(-1); // Start with an empty name stack

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);
    gluPerspective(45.0, (double)viewport[2] / (double)viewport[3], 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);

    draw_solids(); // Render all solids in selection mode

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    hits = glRenderMode(GL_RENDER);

    if (hits > 0) {
        GLuint *ptr = select_buf;
        GLuint closest = ptr[1]; // Minimum depth value
        selected_solid = ptr[3]; // First hit's name
        for (int i = 1; i < hits; i++) {
            ptr += 4;
            if (ptr[1] < closest) {
                closest = ptr[1];
                selected_solid = ptr[3];
            }
        }
    } else {
        selected_solid = -1; // No selection
    }
}


// Mouse click callback function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        pick_solid(x, y);
    }
}

// Idle function to rotate the selected solid
void idle() {
    if (selected_solid >= 0 && selected_solid < 5) {
        rotation_angles[selected_solid] += 1.0;
        if (rotation_angles[selected_solid] >= 360.0) {
            rotation_angles[selected_solid] -= 360.0;
        }
    }
    glutPostRedisplay();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Platonic Solids");

    init_opengl();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

