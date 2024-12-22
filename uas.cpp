#include <GL/glut.h>
#include <math.h>

// Deklarasi fungsi pohon
void pohon();

// Fungsi untuk menggambar silinder
void drawCylinder(GLdouble radius, GLdouble height, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, slices, 3);
    gluDeleteQuadric(quad);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Atur posisi kamera lebih jauh
    gluLookAt(10.0f, 10.0f, 15.0f,  // Posisi kamera lebih jauh dari objek
              0.0f, 0.0f, 0.0f,     // Titik yang dilihat kamera
              0.0f, 1.0f, 0.0f);    // Vektor "up"

    // Gambar grid kubus
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            glPushMatrix();
            glTranslatef(j * 1.5f - 1.5f, 0.0f, i * 1.5f - 1.5f);
            glColor3f(0.5f + 0.1f * i, 0.3f + 0.1f * j, 0.7f);
            glutWireCube(1.0f);
            glPopMatrix();
        }
    }

    // Gambar pohon
    pohon();
    
    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_COLOR_MATERIAL);
}

void pohon() {
    // Gambar batang pohon pertama (wireframe)
    glPushMatrix();
    glColor3d(0.803921568627451, 0.5215686274509804, 0.2470588235294118);
    glTranslated(5.0, 0.0, 0.0);  
    glRotated(90, -1.0, 0.0, 0.0); 
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Set wireframe mode
    drawCylinder(0.2, 2.0, 15);
    glPopMatrix();

    // Gambar daun pohon pertama (wireframe)
    glPushMatrix();
    glColor3d(0.0, 0.8, 0.0);  
    glTranslated(5.0, 2.0, 0.0);  
    glutWireSphere(0.8, 20, 20);  // Menggunakan glutWireSphere instead of glutSolidSphere
    
    glTranslated(0.4, 0.2, 0.0);
    glutWireSphere(0.5, 15, 15);
    
    glTranslated(-0.8, 0.1, 0.3);
    glutWireSphere(0.5, 15, 15);
    
    glTranslated(0.0, -0.1, -0.6);
    glutWireSphere(0.5, 15, 15);
    glPopMatrix();

    // Gambar batang pohon kedua (wireframe)
    glPushMatrix();
    glColor3d(0.803921568627451, 0.5215686274509804, 0.2470588235294118);
    glTranslated(-5.0, 1.0, 0.0);  
    glRotated(90, 1.0, 0.0, 0.0); 
    drawCylinder(0.3, 3.0, 15);
    glPopMatrix();

    // Gambar daun pohon kedua (wireframe)
    glPushMatrix();
    glColor3d(0.0, 0.8, 0.0);  
    glTranslated(-5.0, 2.0, 0.0);  
    glutWireSphere(1.0, 21, 21);  
    
    glTranslated(0.5, 0.3, 0.2);
    glutWireSphere(0.8, 17, 17);
    
    glTranslated(0.8, 0.1, 0.3);
    glutWireSphere(1.0, 17, 17);
    
    glTranslated(0.0, 0.1, 0.6);
    glutWireSphere(1.0, 16, 16);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Reset ke mode fill untuk objek lain
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3x3 Grid of Wireframe Cubes with Spherical Tree");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
