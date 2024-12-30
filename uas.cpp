/*
||================================================================  Library  ===========================================================================||
*/
#include <GL/glut.h>
#include <math.h>
#include <cstdlib>
#include <vector>
using namespace std;


/*
||================================================================  Prototype Function  ===============================================================||
*/
void drawSun(); 
void drawCloud(float x, float y, float z);


/*
||================================================================  Global Scobe Variabel  ============================================================||
*/
struct Position
{
    float x, y, z;
};

// Vektor global untuk menyimpan posisi pohon
vector<Position> treePosition;

// Variabel untuk jumlah pohon dan awan
int numTrees = 50;
int numClouds = 12;
int currentScene = 1;

// Arrays untuk menyimpan posisi pohon dan awan
float treePositions[100][3];  // Array untuk posisi pohon
float cloudPositions[100][3]; // Array untuk posisi awan
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 30.0f;
float cloudRotationAngle = 1.0f;

GLfloat light_position[] = {0.0, 20.0, -15.0, 1.0};
GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};



/*
||================================================================  Fungsi Ground/Lantai  (Rizki)  ======================================================================||
*/
void drawGround()
{
    glPushMatrix();
    glColor3f(0.1f, 0.6f, 0.1f);
    glTranslatef(0.0f, -0.5f, 0.0f);
    glScalef(200.0f, 0.25f, 800.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

/*
||================================================================  Fungsi Matahari (Rizki)  ============================================================================||
*/
void drawSun()
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);       // Yellow color for the sun
    glTranslatef(0.0f, 20.0f, -25.0f); // Position it above the scene
    glutSolidSphere(6.5f, 20, 20);     // Draw sun as a solid sphere
    glPopMatrix();
}

/*
||================================================================  Fungsi Sphere/Bahan Awan (Sulthan)  ================================================================||
*/
// Fungsi untuk menggambar bola (sphere)
void drawSphere(float x, float y, float z, float radius)
{
    glPushMatrix();
    glTranslatef(x, y, z);           // Pindahkan posisi bola
    glutSolidSphere(radius, 20, 20); // Gambar bola padat
    glPopMatrix();
}

/*
||================================================================  Fungsi Awan (Sulthan)  =============================================================================||
*/
// Fungsi untuk menggambar awan
void drawCloud(float x, float y, float z)
{
    glColor3f(1.0f, 1.0f, 1.0f); // Set cloud color to white
    float cloudSize = 0.75f;     // Increase the size of the cloud spheres
    drawSphere(x, y, z, cloudSize);
    drawSphere(x + cloudSize, y, z, cloudSize);
    drawSphere(x - cloudSize, y, z, cloudSize);
    drawSphere(x, y + cloudSize, z, cloudSize);
}

void drawRandomClouds()
{
    // Gambar awan berdasarkan posisi yang sudah di-generate
    for (size_t i = 0; i < cloudPosition.size(); ++i)
    {
        const Position &pos = cloudPosition[i];
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(cloudRotationAngle, 0.0f, 1.0f, 0.0f); // Rotasi awan
        drawCloud(0, 0, 0);
        glPopMatrix();
    }
}

void genRandomClouds(int cloudCount)
{
    // Generate posisi awan hanya sekali
    if (cloudPosition.empty())
    {
        for (int i = 0; i < cloudCount; ++i)
        {
            Position pos;
            generateRandomPosition(pos.x, pos.y, pos.z, 10.0f);
            cloudPosition.push_back(pos);
        }
    }
}

vector<Position> cloudPosition;

// Fungsi untuk menghasilkan posisi acak untuk awan
void generateCloudPositions()
{
    for (int i = 0; i < numClouds; i++)
    {
        cloudPositions[i][0] = (rand() % 40) - 50.0f; // Random X between -20 and 20
        cloudPositions[i][1] = (rand() % 10) + 5.0f;  // Random Y between 5 and 15 (cloud height)
        cloudPositions[i][2] = (rand() % 40) - 50.0f; // Random Z between -20 and 20
    }
}

// Fungsi untuk menggambar awan pada posisi yang sudah disimpan
void generateClouds()
{
    for (int i = 0; i < numClouds; i++)
    {
        // Gunakan posisi awan yang sudah disimpan
        float x = cloudPositions[i][0];
        float y = cloudPositions[i][1];
        float z = cloudPositions[i][2];

        // Gambar awan pada posisi yang sudah disimpan
        drawCloud(x, y, z);
    }
}

/*
||================================================================  Fungsi Cylinder/Bahan Batang Pohon (Fetra)  ========================================================||
*/
// Function to draw a cylinder
void drawCylinder(GLdouble radius, GLdouble height, int slices)
{
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, slices, 3);
    gluDeleteQuadric(quad);
}

/*
||================================================================  Fungsi Pohon (Fetra)  ==============================================================================||
*/
// Fungsi menggambar pohon
void kipDrawTree(float x, float y, float z)
{
    // Draw the tree trunk
    glTranslatef(x, y, z);
    glPushMatrix();
    glRotated(-90, 1.0, 0.0, 0.0);
    glColor3f(0.5f, 0.35f, 0.05f);
    gluCylinder(gluNewQuadric(), 0.5, 0.3, 4.0, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(-20, 1.0, 0.0, 0.0);
    glTranslatef(0, 2.0, 1.0);
    glColor3f(0.5f, 0.35f, 0.05f);
    gluCylinder(gluNewQuadric(), 0.2, 0.1, 0.7, 20, 10);
    glPopMatrix();

    // Draw the tree foliage
    glPushMatrix();
    glTranslatef(0, 4.0, 0);
    glColor3ub(62, 123, 39);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(62, 123, 39);
    glTranslatef(0, 5.0, 0);
    glutSolidSphere(0.9, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8, 4.3, 0);
    glColor3ub(62, 123, 39);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 4.3, 0.8);
    glColor3ub(62, 123, 39);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 4.3, -0.8);
    glColor3ub(62, 123, 39);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.9, 4.3, 0);
    glColor3ub(62, 123, 39);
    glutSolidSphere(0.65, 20, 20);
    glPopMatrix();
}

// Fungsi untuk membuat posisi acak
void generateRandomPosition(float &x, float &y, float &z, float height)
{
    do
    {
        // Generate angka acak untuk x dan z dalam range -20 hingga 20
        x = (rand() % 51 - 25); // -20 hingga 20
        z = (rand() % 51 - 25);
    } while ((x >= -12 && x <= 12) && (z >= -12 && z <= 12)); // Cek radius larangan

    // Y adalah tinggi pohon (ground level)
    y = height;
}


// Fungsi untuk menggambar pohon secara acak
void drawRandomTrees()
{
    // Gambar pohon berdasarkan posisi yang sudah di-generate
    for (size_t i = 0; i < treePosition.size(); ++i)
    {
        const Position &pos = treePosition[i];
        glPushMatrix();
        kipDrawTree(pos.x, pos.y, pos.z);
        glPopMatrix();
    }
}


void genRandomTrees(int treeCount)
{
    // Generate posisi pohon hanya sekali
    if (treePosition.empty())
    {
        for (int i = 0; i < treeCount; ++i)
        {
            Position pos;
            generateRandomPosition(pos.x, pos.y, pos.z, -1.0f);
            treePosition.push_back(pos);
        }
    }
}

void generateTreePositions()
{
    for (int i = 0; i < numTrees; i++)
    {
        treePositions[i][0] = (rand() % 40) - 20.0f; // Random X between -20 and 20
        treePositions[i][1] = 1.0f;                  // Ground level for the tree
        treePositions[i][2] = (rand() % 40) - 20.0f; // Random Z between -20 and 20
    }
}

// Fungsi untuk menggambar pohon pada posisi yang sudah disimpan
void generateTrees()
{
    for (int i = 0; i < numTrees; i++)
    {
        // Gunakan posisi pohon yang sudah disimpan
        float x = treePositions[i][0];
        float y = treePositions[i][1];
        float z = treePositions[i][2];

        // Gambar pohon pada posisi yang sudah disimpan
        glPushMatrix();
        glTranslatef(x, y, z);
        kipDrawTree(float x, float y, float z); // Gambar pohon pada posisi yang sudah disimpan
        glPopMatrix();
    }
}


/*
||================================================================  End Here  =============================================================================================||
*/


/*
||================================================================  Fungsi Pyramid (Rizki)  ===============================================================================||
*/
void drawCubes(int rows, int cols, float height, float offsetX, float offsetZ)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            glPushMatrix();
            glTranslatef(j * 1.0f + offsetX, height, i * 1.0f + offsetZ);
            glColor3ub(243, 158, 96);
            glutSolidCube(1.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            glutWireCube(1.0f);
            glPopMatrix();
        }
    }
}


/*
||================================================================  Fungsi Lainnya  =======================================================================================||
*/
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
	

    // Draw cubes
    for (int level = 0; level < 9; level++)
    {
        int size = 9 - level;
        float height = level * 1.0f;
        float offset = -0.5f * (size - 1);
        drawCubes(size, size, height, offset, offset);
    }


    // Gambar scene berdasarkan pilihan
    if (currentScene == 1)
    {
        // Gambar semua objek di Scene 1
        drawSun();

        for (size_t i = 0; i < treePosition.size(); ++i)
            kipDrawTree(float x, float y, float z);

        for (size_t i = 0; i < cloudPosition.size(); ++i)
            drawCloud(float x, float y, float z)
    }

    cloudRotationAngle += 0.05f;
    if (cloudRotationAngle >= 360.0f)
        cloudRotationAngle -= 360.0f;

    glFlush();
    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay(); // Memanggil redisplay untuk animasi
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

void menu(int option)
{
    switch (option)
    {
    case 1:
        currentScene = 1; // Pindah ke Scene 1
        break;
    case 2:
        currentScene = 2; // Pindah ke Scene 2
        break;
    case 3:
        exit(0); // Keluar
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("tb kelompok 3");

    numTrees = 7;   // Jumlah pohon
    numClouds = 12; // Jumlah awan

    genRandomTrees(20);
    genRandomClouds(30);

    generateTreePositions();
    generateCloudPositions();

	// Membuat menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Scene 1", 1);
    glutAddMenuEntry("Scene 2", 2);
    glutAddMenuEntry("Keluar", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);	
	
	init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle); // Tambahkan fungsi idle
    glutMainLoop();
    return 0;

}

