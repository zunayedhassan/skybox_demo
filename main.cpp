/**
 *  Application Name: Skybox Demo
 *
 *  Date:             April 14, 2015
 *  Author:           Mohammad Zunayed Hassan
 *  Email:            ZunayedHassanBD@gmail.com
 */

#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "camera.h"
#include "model.h"
#include "skybox.h"

using namespace std;

/* Application settings */
string title                  = "Skybox";

int windowWidth               = 640,
    windowHeight              = 480;

bool isFullScreenEnable       = true;

float fieldOfVision           = 60.0f,
      cameraDistance          =  5.0f;

float backgroundColor[3]      = { 0, 0, 0 };                // Color: Black

int timeForUpdatingFrame      = 25;

const int KEY_ESCAPE          = 27;

bool isWireframeModeEnabled   = false;                      // Default: false

/* Lighting Settings */
// Set material properties for lighting
bool isLightingEnabled        = true,
     isSpecularLightEnabled   = true;

float shininess               = 50.0f;                      // Shininess: 0 to 128;

GLfloat colorWhite[]          = { 1.00, 1.00, 1.00, 1.0 };  // Color: White
GLfloat colorDarkGray[]       = { 0.30, 0.30, 0.30, 1.0 };  // Color: Dark gray
GLfloat colorLightGray[]      = { 0.95, 0.95, 0.95, 1.0 };  // Color: Light gray

GLfloat lightPosition[]	      = { 0.5, 1.0, 1.0, 0.0 };

/* Camera Settings */
const float cameraRotationSpeed     = M_PI / 180 * 0.2;
const float cameraTranslationSpeed  = 0.10f;
const float initialCameraHeight     = 1.5f;
bool keypress[256];

Camera *camera = new Camera(0, initialCameraHeight, 0);

// ----------------------------------
/* Skybox Settings */
const string skyboxPathName = "C:\\Users\\Zunayed Hassan\\QT\\hello_opengl_001\\skybox";
Skybox *skybox = nullptr;
// ----------------------------------


void DrawGrid()
{
    glPushMatrix();
    glColor3f(1, 1, 1);

    for (int i= -50; i < 50; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);
        glEnd();
    }

    for (int i = -50; i < 50; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
        glEnd();
    }

    glPopMatrix();
}

void DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);                             // Switch to the drawing perspective
    glLoadIdentity();                                       // Reset the drawing perspective

    camera->Refresh();

    glTranslatef(0.0f, 0.0f, -cameraDistance);              // Move forward 5 units

    // Adding light
    if (isLightingEnabled)
    {
        // Set lighting intensity and color
        glLightfv(GL_LIGHT0, GL_AMBIENT,  colorDarkGray );
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  colorLightGray);

        if (isSpecularLightEnabled)
        {
            glLightfv(GL_LIGHT0, GL_SPECULAR, colorWhite);

            // Setting material properties
            glMaterialfv(GL_FRONT, GL_SPECULAR, colorWhite);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess); // Shininess: 0 to 128
        }

        // Set the light position
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    }

    // Your code here...
    glPushMatrix();

    // Drawing a cube
    glColor3f(0.20, 0.61, 0.94);                            // Color: Cornflower blue
    glutSolidTeapot(0.50);

    glPopMatrix();

    // ----------------------------------
    skybox->Draw();
    // ----------------------------------

    glTranslatef(0, -0.5, 0);
    DrawGrid();

    glutSwapBuffers();
}

void Update(int value)
{
    glutPostRedisplay();                                    // Tell GLUT that the display has changed

    if(keypress['w'] || keypress['W'])
    {
        camera->Move(cameraTranslationSpeed);
    }
    else if(keypress['s'] || keypress['S'])
    {
        camera->Move(-cameraTranslationSpeed);
    }
    else if(keypress['a'] || keypress['A'])
    {
        camera->Strafe(cameraTranslationSpeed);
    }
    else if(keypress['d'] || keypress['D'])
    {
        camera->Strafe(-cameraTranslationSpeed);
    }

    camera->m_y = initialCameraHeight;

    // Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(timeForUpdatingFrame, Update, 0);
}

// Called when a key is pressed
void HandleKeypress(unsigned char key, int x, int y) {
    switch (key)
    {
        // Escape key
        case KEY_ESCAPE:
            exit(EXIT_SUCCESS);
            break;

        default:
            keypress[key] = true;
            break;
    }
}

void KeyboardUp(unsigned char key, int x, int y)
{
    keypress[key] = false;
}

void MouseMotion(int x, int y)
{
    // This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
    // This avoids it being called recursively and hanging up the event loop
    static bool just_warped = false;

    if (just_warped) {
        just_warped = false;
        return;
    }

    int dx = x - windowWidth / 2;
    int dy = y - windowHeight / 2;

    if (dx)
    {
        camera->RotateYaw(cameraRotationSpeed * dx);
    }

    if (dy)
    {
        camera->RotatePitch(cameraRotationSpeed * dy);
    }

    glutWarpPointer(windowWidth / 2, windowHeight / 2);

    just_warped = true;
}

// Called when the window is resized
void HandleResize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfVision, (double) width / (double) height, 1.0, 200.0);
}

int main(int argc, char* argv[])
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);

    //Create the window
    glutCreateWindow(title.c_str());

    // Initialize
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
    glClearDepth(1.0f);                                 // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                            // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                             // Set the type of depth-test
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);                            // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    glEnable(GL_TEXTURE_2D);

    if (isFullScreenEnable)
    {
        windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        glutFullScreen();
    }

    // Lighting set up
    if (isLightingEnabled)
    {
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }

    if (isWireframeModeEnabled)
    {
        // Turn on wireframe mode
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
    }

    // Set handler functions
    glutDisplayFunc(DrawScene);
    glutReshapeFunc(HandleResize);

    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(windowWidth / 2, windowHeight / 2);

    glutKeyboardFunc(HandleKeypress);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MouseMotion);
    glutKeyboardUpFunc(KeyboardUp);

    camera->RotateYaw(-1.5);

    // ----------------------------------------
    skybox = new Skybox(skyboxPathName);
    // ----------------------------------------

    glutTimerFunc(timeForUpdatingFrame, Update, 0);     // Add a timer

    glutMainLoop();
    return EXIT_SUCCESS;
}
