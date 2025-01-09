/*
||================================================================  Library  ==========================================================================||
*/
#include <GL/glut.h>
#include <math.h>
#include <cstdlib>
using namespace std;

/*
||================================================================  Global Scobe Variabel  ============================================================||
*/

int currentScene = 1; // Default scene
int numTrees = 90;
int numClouds = 12;
int numStars = 200; // Jumlah bintang

// Arrays untuk menyimpan posisi pohon dan awan
float treePositions[100][3];  // Array untuk posisi pohon
float cloudPositions[100][3]; // Array untuk posisi awan
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 30.0f;
float cloudRotationAngle = 1.0f;
float starPositions[100][3]; // Maksimum 100 bintang
float scaleFactor = 1.0f; // Variabel untuk skala objek di malam hari
float sunRotationAngle = 0.0f; // Variabel untuk sudut rotasi matahari
float transitionTime = 0.0f; // Variabel untuk mengatur waktu transisi
float moonRotationAngle = 0.0f; // Variabel untuk sudut rotasi bulan
bool isDay = true; // Untuk menentukan siang atau malam
bool showCartesian = true;

GLfloat light_position[] = {0.0, 20.0, -15.0, 1.0};
GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

/*
||================================================================  Fungsi Objek Pyramid (Rizki)  ==================================================================||
*/
void drawCubes(int rows, int cols, float height, float offsetX, float offsetZ)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            glPushMatrix();
            glLineWidth(2.0f);
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
||================================================================  Fungsi Objek Tanah/Lantai (Rizki)  ============================================================||
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
||================================================================  Fungsi Objek Bulan (Sulthan)  =================================================================||
*/
void drawMoon()
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);       // Warna putih untuk bulan
    glRotatef(moonRotationAngle, 1.0f, 0.0f, 0.0f); // Rotasi bulan di sumbu X
    glTranslatef(0.0f, 20.0f, -15.0f); // Posisi di atas scene
    glScalef(0.5f, 0.5f, 0.5f);        // Perkecil ukuran bulan
    glutSolidSphere(6.5f, 20, 20);     // Gambar bulan sebagai bola padat
    glPopMatrix();
}

/*
||================================================================  Fungsi Objek Awan (Sulthan)  ==================================================================||
*/
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
    glColor3f(1.0f, 1.0f, 1.0f); // Set cloud color to white
    float cloudSize = 0.75f;     // Increase the size of the cloud spheres
    drawSphere(x, y, z, cloudSize);
    drawSphere(x + cloudSize, y, z, cloudSize);
    drawSphere(x - cloudSize, y, z, cloudSize);
    drawSphere(x, y + cloudSize, z, cloudSize);
}

// Fungsi untuk menghasilkan posisi acak untuk awan
void generateCloudPositions()
{
    for (int i = 0; i < numClouds; i++)
    {
        cloudPositions[i][0] = (rand() % 51 - 25);   // Random X between -20 and 20
        cloudPositions[i][1] = (rand() % 10) + 5.0f; // Random Y between 5 and 15 (cloud height)
        cloudPositions[i][2] = (rand() % 51 - 25);   // Random Z between -20 and 20
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
||================================================================  Fungsi Objek Matahari (Fetra)  ==============================================================||
*/
void drawSun()
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);       // Warna kuning untuk matahari
    glRotatef(sunRotationAngle, 1.0f, 0.0f, 0.0f); // Rotasi matahari di sumbu X
    glTranslatef(0.0f, 20.0f, -15.0f); // Posisi di atas scene
    glScalef(0.5f, 0.5f, 0.5f);        // Perkecil ukuran matahari
    glutSolidSphere(6.5f, 20, 20);     // Gambar matahari sebagai bola padat
    glPopMatrix();
}

/*
||================================================================  Fungsi Objek Pohon (Fetra)  =================================================================||
*/
	void drawTree()
{
    // Draw the tree trunk
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

void generateTreePositions()
{
    for (int i = 0; i < numTrees; i++)
    {
        float x, z;
        do
        {
            x = (rand() % 40) - 20.0f; // Random X between -20 and 20
            z = (rand() % 40) - 20.0f; // Random Z between -20 and 20
        } while (sqrt(x * x + z * z) <= 14.0f); // Ensure the tree is outside the 12 unit radius

        treePositions[i][0] = x;
        treePositions[i][1] = -1.0f; // Ground level for the tree
        treePositions[i][2] = z;
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
        drawTree(); // Gambar pohon pada posisi yang sudah disimpan
        glPopMatrix();
    }
}

/*
||================================================================  Fungsi Star (Rizki)  ========================================================================||
*/

void drawStars() {
    glPushMatrix();
    glPointSize(2.0f); // Ukuran bintang
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // Warna bintang (putih)

    for (int i = 0; i < numStars; i++) {
        glVertex3f(starPositions[i][0], starPositions[i][1], starPositions[i][2]);
    }

    glEnd();
    glPopMatrix();
}


void generateStarPositions() {
    for (int i = 0; i < numStars; i++) {
        // X: Lebih luas (-50.0 hingga 50.0)
        starPositions[i][0] = (rand() % 100 - 50) / 1.0f; 

        // Y: Tinggi di langit (20.0 hingga 50.0)
        starPositions[i][1] = (rand() % 30 + 10) / 1.0f;  

        // Z: Lebih luas (-50.0 hingga 50.0)
        starPositions[i][2] = (rand() % 100 - 50) / 1.0f;
    }
}

/*
||================================================================  Fungsi Cartesian (Rizki)  ==================================================================||
*/
void drawCartesian()
{
    if (showCartesian)
        return;

    glPushMatrix();
    glLineWidth(2.0f);

    // Gambar sumbu X
    glColor3f(1.0f, 1.0f, 1.0f); // Merah untuk X
    glBegin(GL_LINES);
    glVertex3f(-50.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);
    glEnd();

    // Gambar sumbu Y
    glColor3f(1.0f, 1.0f, 1.0f); // Hijau untuk Y
    glBegin(GL_LINES);
    glVertex3f(0.0f, -50.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);
    glEnd();

    // Gambar sumbu Z
    glColor3f(1.0f, 1.0f, 1.0f); // Biru untuk Z
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -50.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);
    glEnd();

    glPopMatrix();
}

/*
||================================================================  Fungsi Scene 2D (Sulthan)  ==================================================================||
*/
void scene1() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(0.0, 5.0, 30.0,  // Posisi kamera
              0.0, 0.0, 0.0,  // Titik yang dilihat kamera
              0.0, 1.0, 0.0); // Arah atas kamera

    // Nonaktifkan pencahayaan
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    // Lantai
    glPushMatrix();
    glColor3f(0.1f, 0.6f, 0.1f);
    glTranslatef(0.0f, -0.5f, 0.0f);
    glScalef(200.0f, 0.25f, 800.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tambahkan pohon 2D
    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        glColor3f(0.5, 0.35, 0.05); // Warna batang pohon
        glBegin(GL_QUADS);
        glVertex3f(-1.0 + i * 5, 0.0, -30.0);
        glVertex3f(-0.5 + i * 5, 0.0, -30.0);
        glVertex3f(-0.5 + i * 5, 3.0, -30.0);
        glVertex3f(-1.0 + i * 5, 3.0, -30.0);
        glEnd();
        glColor3f(0.0, 0.8, 0.0); // Warna daun
        glBegin(GL_TRIANGLES);
        glVertex3f(-1.25 + i * 5, 3.0, -30.0);
        glVertex3f(-0.25 + i * 5, 3.0, -30.0);
        glVertex3f(-0.75 + i * 5, 5.0, -30.0);
        glEnd();
        glPopMatrix();
    }

    // Tambahkan awan 2D
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0); // Warna awan
        glBegin(GL_QUADS);
        glVertex3f(-20.0 + i * 10, 15.0, -40.0);
        glVertex3f(-15.0 + i * 10, 15.0, -40.0);
        glVertex3f(-15.0 + i * 10, 17.0, -40.0);
        glVertex3f(-20.0 + i * 10, 17.0, -40.0);
        glEnd();
        glPopMatrix();
    }

    // Piramida
    glPushMatrix();
    glColor3f(0.8, 0.6, 0.4);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0, 10.0, 0.0);   // Puncak piramida
    glVertex3f(-10.0, 0.0, -10.0); // Basis kiri belakang
    glVertex3f(10.0, 0.0, -10.0);  // Basis kanan belakang

    glVertex3f(0.0, 10.0, 0.0);   // Puncak piramida
    glVertex3f(10.0, 0.0, -10.0);  // Basis kanan belakang
    glVertex3f(10.0, 0.0, 10.0);   // Basis kanan depan

    glVertex3f(0.0, 10.0, 0.0);   // Puncak piramida
    glVertex3f(10.0, 0.0, 10.0);   // Basis kanan depan
    glVertex3f(-10.0, 0.0, 10.0);  // Basis kiri depan

    glVertex3f(0.0, 10.0, 0.0);   // Puncak piramida
    glVertex3f(-10.0, 0.0, 10.0);  // Basis kiri depan
    glVertex3f(-10.0, 0.0, -10.0); // Basis kiri belakang
    glEnd();
    glPopMatrix();
    
    // Matahari terbenam
    glPushMatrix();
    glColor3f(1.0, 0.8, 0.0);
    glTranslatef(0.0, 15.0, -30.0); // Posisikan matahari
    glutSolidSphere(3.0, 20, 20);   // Gambar matahari
    glPopMatrix();

    glutSwapBuffers();
}

/*
||================================================================  Fungsi Scene Siang (Fetra)  ==================================================================||
*/
void siang(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update waktu transisi
    transitionTime += 0.01f; // Atur kecepatan transisi
    if (transitionTime > 1.0f) transitionTime = 1.0f; // Batas maksimum

    // Mengatur cahaya
    GLfloat light_position[] = {0.0, 20.0, -15.0, 1.0}; // Sesuaikan posisi cahaya
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    sunRotationAngle -= -0.1f; // Kurangi sudut rotasi untuk muter ke bawah
    if (sunRotationAngle < 1.0f) // Reset jika kurang dari 0
        sunRotationAngle += -360.0f;

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
    
    // Gambar sumbu kartesius
    drawCartesian();

    glPushMatrix();
	glScalef(scaleFactor, scaleFactor, scaleFactor); // Terapkan skala di malam hari

    // Draw cubes
    for (int level = 0; level < 9; level++)
    {
        int size = 9 - level;
        float height = level * 1.0f;
        float offset = -0.5f * (size - 1);
        drawCubes(size, size, height, offset, offset);
    }
    glPopMatrix();

    // Generate trees at pre-determined positions
    generateTrees();

    // drawRandomTrees();

    glPushMatrix();
    glRotatef(cloudRotationAngle, 0.0f, 1.0f, 0.0f); // Rotasi awan
    generateClouds();
    glPopMatrix();

    cloudRotationAngle += 0.05f;
    if (cloudRotationAngle >= 360.0f)
        cloudRotationAngle -= 360.0f;
        
    // Draw the sun
    drawSun();
	glClearColor(0.5f, 0.5f, 1.0f, 0.0f); // Correct clear color
    // Generate clouds at pre-determined positions

    glFlush();
    glutSwapBuffers();
}

/*
||================================================================  Fungsi Scene Malam (Rizki)  ==================================================================||
*/
void malam(){
	 // Update sudut rotasi bulan
    moonRotationAngle -= -0.1f; // Kurangi sudut rotasi untuk muter ke bawah
    if (moonRotationAngle < 1.0f) // Reset jika kurang dari 0
        moonRotationAngle += -360.0f;

	
    // Update waktu transisi
    transitionTime -= 0.007f; // Atur kecepatan transisi
    if (transitionTime < 0.0f) transitionTime = 0.0f; // Batas minimum

    // Mengatur warna latar belakang (gradasi)
    glClearColor(2.2f * transitionTime, 0.7f * transitionTime, 0.4f * transitionTime, -3.0f);

    // Mengatur cahaya
    GLfloat light_position[] = {0.0, 20.0 * transitionTime, -15.0, 1.0}; // Sesuaikan posisi cahaya
	GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat light_diffuse[] = {1.0, 0.7, 2.0, 0.5};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

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
    
    // Gambar sumbu kartesius
    drawCartesian();

    glPushMatrix();
    glScalef(scaleFactor, scaleFactor, scaleFactor); // Terapkan skala di malam hari

    // Draw cubes
    for (int level = 0; level < 9; level++)
    {
        int size = 9 - level;
        float height = level * 1.0f;
        float offset = -0.5f * (size - 1);
        drawCubes(size, size, height, offset, offset);
    }
    glPopMatrix();

    // Generate trees at pre-determined positions
    generateTrees();

    glPushMatrix();
    glRotatef(cloudRotationAngle, 0.0f, 1.0f, 0.0f); // Rotasi awan
    generateClouds();
    glPopMatrix();

    cloudRotationAngle += 0.05f;
    if (cloudRotationAngle >= 360.0f)
        cloudRotationAngle -= 360.0f;

    // Draw the sun
    drawMoon();
	
    // Generate clouds at pre-determined positions
	
	drawStars();
    
    glFlush();
    glutSwapBuffers();
}

/*
||================================================================  Fungsi Keyboard (Fetra)  ============================================================================||
*/
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
    case 'b': // Perbesar skala
        scaleFactor += 0.1f;
        if (scaleFactor > 5.0f)
            scaleFactor = 5.0f; // Maksimum skala
        break;
    case 'k': // Perkecil skala
        scaleFactor -= 0.1f;
        if (scaleFactor < 0.1f)
            scaleFactor = 0.1f; // Minimum skala
        break;
	case 'c': // Toggle Cartesian plane
        showCartesian = !showCartesian;
        break;
        
    case 27: // Escape key
        exit(0);
        break;
    }    
	glutPostRedisplay();
}

/*
||================================================================  Fungsi Reshape (Rizki)   ============================================================================||
*/

// Reshape function to maintain aspect ratio when window is resized
void reshape(int w, int h)
{
    if (h == 0) h = 1; // Avoid division by zero
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Function to toggle fullscreen
void toggleFullscreen()
{
    static bool isFullscreen = false;
    isFullscreen = !isFullscreen;

    if (isFullscreen)
    {
        glutFullScreen(); // Switch to fullscreen
    }
    else
    {
        glutReshapeWindow(800, 600); // Switch back to windowed mode with default size
        glutPositionWindow(100, 100); // Position window
    }
}


/*
||================================================================  Display (Sulthan)   ==============================================================================||
*/
void display()
{
  	// Gambar scene berdasarkan pilihan
    if (currentScene == 1)
    {
        // Gambar semua objek di Scene 1
        scene1();
    }
	else if (currentScene == 2)
	{
		siang();
	}
	else if (currentScene == 3)
	{
		malam();
	}
}

/*
||================================================================  Fungsi Menu GUI (Fetra)   ================================================================================||
*/
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
        currentScene = 3; // Pindah ke Scene 3
        break;
    case 4:
        exit(0); // Keluar
    }
    glutPostRedisplay();
}

/*
||================================================================  Fungsi Init (Sulthan)   ================================================================================||
*/
void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 1.0f, 0.0f); // Correct clear color
}

/*
||================================================================  Fungsi Lainnya   ================================================================================||
*/
void idle()
{
    glutPostRedisplay(); // Memanggil redisplay untuk animasi
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("tb kelompok 3");

	int numTrees = 90;
	int numClouds = 12;
	int numStars = 200; // Jumlah bintang

    generateTreePositions();
    generateCloudPositions();
    generateStarPositions(); // Tambahkan ini
    
    // Membuat menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Scene 1", 1);
    glutAddMenuEntry("Scene 2", 2);
    glutAddMenuEntry("Scene 3", 3);
    glutAddMenuEntry("Keluar", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle); // Tambahkan fungsi idle
    glutReshapeFunc(reshape); // Register reshape function

    glutMainLoop();
    return 0;
}
