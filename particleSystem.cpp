#include <iostream>
#include <list>
#include "MersenneTwister.h"

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
    vector3 randDisplacement;
    float age;
    float color[4];
};

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;  // Angles to rotate scene.
static float startDelay = 1000; // Draw delay
static vector3 startPosition = {0.0, 10.0, 0.0};

// User can manipulate these
static vector3 displacement = {0.0, 0.0, 0.0};
static vector3 randDisplacementMin = {-0.01, -0.01, -0.01};
static vector3 randDisplacementMax = {0.01, 0.01, 0.01};
static unsigned int redrawTime = 100;
static float lifeTime = 4000;
static float particleRadius = 1.0f;
static bool randomColor = true;
static float particleColor[] = {1.0, 0.0, 0.0, 1.0};

list<particle> parts;

// Drawing routine.
void drawScene(void)
{
    // Light position vectors.	
    float lightPos0[] = { 1.0, 1.0, 0.0, 0.0 };
    
    // Material property vectors.
    float matAmb[] = {0.0, 0.0, 1.0, 1.0};
    float matDif[] = {0.0, 0.0, 1.0, 1.0};
    float matSpec[] = { 0.0, 0.0, 0.0, 1.0 };
    float matShine[] = { 50.0 };
    float matEmission[] = {0.0, 0.0, 0.0, 1.0};
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
         
     // Rotate scene.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);
    
    glDisable(GL_LIGHTING);

    // Light0 is positioned.
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);   
    
    for(list<particle>::iterator i = parts.begin(); i != parts.end(); i++)
    {
        // Material properties of particle.
        if (randomColor)
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT, i->color);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, i->color);
        }
        else
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT, particleColor);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, particleColor);
        }
        glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
        glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
        glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
        glPushMatrix();    
        glTranslatef(i->position.x, i->position.y, i->position.z);
        i->position.x += displacement.x + i->randDisplacement.x;
        i->position.y += displacement.y + i->randDisplacement.y;
        i->position.z += displacement.z + i->randDisplacement.z;
        glutSolidSphere(particleRadius, 20.0, 20.0);
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

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);

    // Light property vectors.
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec0[] = { 1.0, 1.0, 1.0, 1.0 };
    float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };
   
    // Light0 properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);

    glEnable(GL_LIGHT0); // Enable particular light source.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint

    // Cull back faces.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);

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
    p.position.x = startPosition.x;
    p.position.y = startPosition.y;
    p.position.z = startPosition.z;
    MTRand rand;
    p.randDisplacement.x = rand.randDblExc(randDisplacementMax.x - randDisplacementMin.x) + randDisplacementMin.x;
    p.randDisplacement.y = rand.randDblExc(randDisplacementMax.y - randDisplacementMin.y) + randDisplacementMin.y;
    p.randDisplacement.z = rand.randDblExc(randDisplacementMax.z - randDisplacementMin.z) + randDisplacementMin.z;
    if (randomColor)
    {
        p.color[0] = rand.randDblExc();
        p.color[1] = rand.randDblExc();
        p.color[2] = rand.randDblExc();
        p.color[3] = 1.0;
    }
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
    glutInitWindowSize(1000, 1000);
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


