#include <GL/glut.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Declaration of functions
void pohon();
void drawSun();
void drawGround();
void drawCubes(int rows, int cols, float height, float offsetX, float offsetZ);
void drawCloud(float x, float y, float z);
void generateTrees(int count);
void generateClouds(int count);

float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 30.0f;
float cloudRotation = 0.0f; // Rotasi awan

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
            glTranslatef(offsetX + i, height, offsetZ + j);
            glColor3f(0.5f, 0.3f, 0.1f); // Warna cube
            glutSolidCube(1.0f);
            glColor3f(0.0f, 0.0f, 0.0f); // Warna cube
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
    glutSolidSphere(1, 20, 20); // Gambar bola padat
    glPopMatrix();
}

// Fungsi untuk menggambar awan
void drawCloud(float x, float y, float z)
{
    float scaleFactor = 3.0f; // Faktor untuk memperlebar radius rotasi
    glPushMatrix();
    glTranslatef(0.0f, y, 0.0f);            // Naikkan awan ke ketinggian tertentu
    glRotatef(cloudRotation, 0.0f, 1.0f, 0.0f); // Rotasi mengelilingi sumbu Y
    glTranslatef(x * scaleFactor, 0.0f, z * scaleFactor); // Perlebar radius rotasi
    glColor3f(1.0f, 1.0f, 1.0f);           // Warna awan
    drawSphere(0.0f, 0.0f, 0.0f, 1.5f);
    drawSphere(1.0f, 0.0f, 0.0f, 1.0f);
    drawSphere(-1.0f, 0.0f, 0.0f, 1.0f);
    drawSphere(0.0f, 0.5f, 0.0f, 1.0f);
    glPopMatrix();
}

void generateClouds(int count) {
    for (int i = 0; i < count; ++i) {
        float x = static_cast<float>(rand() % 50 - 25); // Rentang -25 hingga 25
        float y = static_cast<float>(rand() % 10 + 5);  // Rentang 5 hingga 15 (ketinggian)
        float z = static_cast<float>(rand() % 50 - 25); // Rentang -25 hingga 25
        drawCloud(x, y, z);
    }
}

void generateTrees(int count) {
    for (int i = 0; i < count; ++i) {
        float x = static_cast<float>(rand() % 50 - 25); // Rentang -25 hingga 25
        float z = static_cast<float>(rand() % 50 - 25); // Rentang -25 hingga 25

        // Batang pohon
        glPushMatrix();
        glColor3d(0.8, 0.5, 0.25);  // Warna batang
        glTranslatef(x, 1.0, z);    // Posisi batang
        glRotatef(90, 1.0, 0.0, 0.0); 
        drawCylinder(0.3, 8.0, 20); // Batang pohon
        glPopMatrix();

        // Daun pohon
        glPushMatrix();
        glColor3d(0.0, 0.8, 0.0);   // Warna daun
        glTranslatef(x, 8.0, z);    // Posisi daun di atas batang
        glutWireSphere(1.5, 20, 20); // Daun utama
        glTranslatef(0.5, 0.5, 0.0);
        glutWireSphere(1.0, 15, 15); // Daun tambahan
        glTranslatef(-1.0, 0.2, 0.3);
        glutWireSphere(1.0, 15, 15); // Daun tambahan
        glPopMatrix();
    }
}

void timer(int value)
{
    cloudRotation += 0.01f; // Tambahkan rotasi
    if (cloudRotation > 360.0f)
        cloudRotation -= 360.0f;

    glutPostRedisplay();           // Perbarui tampilan
    glutTimerFunc(16, timer, 0);   // Panggil lagi dalam 16 ms
}

void display() {
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

    // Generate and draw trees
    generateTrees(20); // Gambar 20 pohon secara acak

    // Draw cubes
    for (int level = 0; level < 9; level++) {
        int size = 9 - level;
        float height = level * 1.0f;
        float offset = -0.5f * (size - 1);
        drawCubes(size, size, height, offset, offset);
    }

    // Draw the sun
    drawSun();

    // Generate and draw clouds
    glPushMatrix();
    glRotatef(cloudRotation, 0.0f, 1.0f, 0.0f); // Rotasi hanya untuk awan
    generateClouds(15); // Gambar 15 awan secara acak
    glPopMatrix();

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

void pohon() {
    //gambar pohon pertama
   glPushMatrix();
    glColor3d(0.8, 0.5, 0.25);
    glTranslated(20.0, 1.0, 0.0);  
    glRotated(90, 8.0, 0.0, 0.0); 
    drawCylinder(0.3, 8.0, 20.0);
    glPopMatrix();

   //gambar daun pohon pertama
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

   //gambar pohon ke dua
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

int main(int argc, char **argv)
{
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Proyek OpenGL Kelompok 3");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0); // Inisialisasi timer
    glutMainLoop();
    return 0;
}

