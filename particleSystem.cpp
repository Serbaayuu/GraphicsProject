#include <iostream>
#include <list>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

struct vector3
{
    float x;
    float y;
    float z;
};

struct particle
{
    vector3 position;
    float age;
};

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static vector3 displacement = {0.0, 0.01, 0.0};
static float position = 0.0;
static unsigned int redrawTime = 500;
static float lifeTime = 4000;
static float startDelay = 1000;

list<particle> parts;

// Drawing routine.
void drawScene(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
         
     // Rotate scene.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);
    
    glColor3f(0.0, 200.0, 205.0);
    
    for(list<particle>::iterator i = parts.begin(); i != parts.end(); i++)
    {
        glPushMatrix();
        glTranslatef(i->position.x, i->position.y, i->position.z);
        i->position.x += displacement.x;
        i->position.y += displacement.y;
        i->position.z += displacement.z;
        glutSolidSphere(1.0, 20.0, 20.0);
        glPopMatrix();
        if(i->age > lifeTime)
        {
            parts.erase(i);
        }
        else
        {
            i->age++;
        }
    }
    
    glFlush();
    glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
    // set the background/clearing color
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key) 
    {
        case 27:
            exit(0);
            break;
        case 'x':
            Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    
}

// Timer redraw
void newParticle(int value)
{
    particle p;
    p.position.x = 0;
    p.position.y = 0;
    p.position.z = 0;
    p.age = 0;
    parts.push_back(p);
    glutPostRedisplay();
    glutTimerFunc(redrawTime, newParticle, 1);
}

// Main routine.
int main(int argc, char **argv) 
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("Particle System"); 
    setup(); 
    glutDisplayFunc(drawScene); 
    glutReshapeFunc(resize);  
    glutKeyboardFunc(keyInput);
    glutTimerFunc(startDelay, newParticle, 1);
    glutMainLoop(); 

    return 0;
}


