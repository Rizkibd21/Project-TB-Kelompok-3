#include <GL/glut.h>
#include <math.h>

// Declaration of tree function
void pohon();
void drawSun(); // Declaration of the sun drawing function

float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 30.0f;

GLfloat light_position[] = {0.0, 20.0, -15.0, 1.0};
GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

// Function to draw a cylinder
void drawCylinder(GLdouble radius, GLdouble height, int slices)
{
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, slices, 3);
    gluDeleteQuadric(quad);
}

void drawCubes(int rows, int cols, float height, float offsetX, float offsetZ)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            glPushMatrix();
            glTranslatef(j * 1.0f + offsetX, height, i * 1.0f + offsetZ);
            // glColor3f(1.0f + 1.0f * i, 1.0f + 1.0f * j, 1.0f);
            glColor3ub(243, 158, 96);
            glutSolidCube(1.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            glutWireCube(1.0f);
            glPopMatrix();
        }
    }
}

void drawGround()
{
    glPushMatrix();
    glColor3f(0.1f, 0.6f, 0.1f);
    glTranslatef(0.0f, -0.5f, 0.0f);
    glScalef(200.0f, 0.25f, 800.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawSun()
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);       // Yellow color for the sun
    glTranslatef(0.0f, 20.0f, -25.0f); // Position it above the scene
    glutSolidSphere(6.5f, 20, 20);     // Draw sun as a solid sphere
    glPopMatrix();
}

// Fungsi untuk menggambar bola (sphere)
void drawSphere(float x, float y, float z, float radius)
{
    glPushMatrix();
    glTranslatef(x, y, z);           // Pindahkan posisi bola
    glutSolidSphere(radius, 20, 20); // Gambar bola padat
    glPopMatrix();
}

// Fungsi untuk menggambar awan
void drawCloud(float x, float y, float z)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    drawSphere(x, y, z, 0.1f);
    drawSphere(x + 0.1f, y, z, 0.1f);
    drawSphere(x - 0.1f, y, z, 0.1f);
    drawSphere(x, y + 0.1f, z, 0.1f);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    float camX = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
    float camY = cameraDistance * sin(cameraAngleX);
    float camZ = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    drawGround();

    // Draw cubes
    for (int level = 0; level < 9; level++)
    {
        int size = 9 - level;
        float height = level * 1.0f;
        float offset = -0.5f * (size - 1);
        drawCubes(size, size, height, offset, offset);
    }

    // Draw trees
    pohon();

    // Draw the sun
    drawSun();

    // Draw cloud
    drawCloud(-2.5f, 10.0f, -10.0f); // Tinggi awan diatur menjadi 10.0f
    drawCloud(-2.4f, 10.5f, -10.0f);
    drawCloud(-2.3f, 10.0f, -10.0f);
    glFlush();
    glutSwapBuffers();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 1.0f, 0.0f); // Correct clear color
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's':
    case 'S':
        cameraAngleX -= 0.05f;
        if (cameraAngleX < -1.5f)
            cameraAngleX = -1.5f;
        break;
    case 'w':
    case 'W':
        cameraAngleX += 0.05f;
        if (cameraAngleX > 1.5f)
            cameraAngleX = 1.5f;
        break;
    case 'a':
    case 'A':
        cameraAngleY -= 0.05f;
        break;
    case 'd':
    case 'D':
        cameraAngleY += 0.05f;
        break;
    case 'z': // Zoom in
        cameraDistance -= 1.0f;
        if (cameraDistance < 5.0f)
            cameraDistance = 5.0f; // Minimum distance
        break;
    case 'x': // Zoom out
        cameraDistance += 1.0f;
        break;
    }
    glutPostRedisplay();
}

void pohon()
{
    // gambar pohon pertama
    glPushMatrix();
    glColor3d(0.8, 0.5, 0.25);
    glTranslated(20.0, 1.0, 0.0);
    glRotated(90, 8.0, 0.0, 0.0);
    drawCylinder(0.3, 8.0, 20.0);
    glPopMatrix();

    // gambar daun pohon pertama
    glPushMatrix();
    glColor3d(0.0, 0.8, 0.0);
    glTranslated(20.0, 2.0, -0.0);
    glutWireSphere(0.8, 20, 20);

    glTranslated(0.4, 0.2, 0.0);
    glutWireSphere(0.5, 15, 15);

    glTranslated(-0.8, 0.1, 0.3);
    glutWireSphere(0.5, 15, 15);

    glTranslated(0.0, -0.1, -0.6);
    glutWireSphere(0.5, 15, 15);
    glPopMatrix();

    // gambar pohon ke dua
    glPushMatrix();
    glColor3d(0.8, 0.5, 0.25);
    glTranslated(-20.0, 1.0, 0.0);
    glRotated(90, 1.0, 0.0, 0.0);
    drawCylinder(0.3, 8.0, 15.0);
    glPopMatrix();

    // Draw foliage for the second tree
    glPushMatrix();
    glColor3d(0.0, 0.8, 0.0);
    glTranslated(-20.0, 2.0, 0.0);
    glutWireSphere(1.0, 21, 21);

    glTranslated(0.5, 0.3, 0.2);
    glutWireSphere(0.8, 17, 17);

    glTranslated(0.8, 0.1, 0.3);
    glutWireSphere(1.0, 17, 17);

    glTranslated(0.0, 0.1, 0.6);
    glutWireSphere(1.0, 16, 16);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("tb kelompok 3");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
