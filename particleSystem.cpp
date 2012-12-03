#include <iostream>
#include <list>
#include "MersenneTwister.h"
#include <string>
#include <vector>

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

static float mousex, mousey;

struct slider{
    float y, linex, liney;
    bool sliding;
};

// Globals.
static float startDelay = 1000; // Draw delay
static vector3 startPosition = {0.0, 10.0, 0.0};
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection
static float lineLength = 3.0;


// User can manipulate these
static vector3 displacement = {0.0, 0.0, 0.0};
static vector3 randDisplacementMin = {-0.01, -0.01, -0.01};
static vector3 randDisplacementMax = {0.01, 0.01, 0.01};
static unsigned int redrawTime = 100;
static float lifeTime = 4000;
static float particleRadius = 1.0f;
static bool randomColor = false;
static float particleColor[] = {1.0, 0.8, 0.8, 1.0};

list<particle> parts;
vector<slider> slides;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, string string)
{  
   int i;
   for (i = 0; i < string.length(); i++)
   {
       glutBitmapCharacter(font, string[i]);
   }
}

void newSlider(float linex, float liney){
    slider s;
    s.y = liney - lineLength / 2;
    s.linex = linex;
    s.liney = liney;
    s.sliding = false;
    slides.push_back(s);
}

// Write data.
void drawGUI(void)
{
   glColor3f(0.0, 0.0, 0.0); 
   
   glRasterPos3f(-45.0, -35.0, 0.0);
   writeBitmapString((void*)font, "Particle Speed    x:       y:       z:");
   
   glRasterPos3f(-45.0, -40.0, 0.0);
   writeBitmapString((void*)font, "Random Speed Min  x:       y:       z:");
   
   glRasterPos3f(-45.0, -45.0, 0.0);
   writeBitmapString((void*)font, "Random Speed Max  x:       y:       z:");
   
   glRasterPos3f(-5.0, -35.0, 0.0);
   writeBitmapString((void*)font, "Redraw time");
   
   glRasterPos3f(-5.0, -40.0, 0.0);
   writeBitmapString((void*)font, "Lifetime");
   
   glRasterPos3f(-5.0, -45.0, 0.0);
   writeBitmapString((void*)font, "Particle size");
   
   glRasterPos3f(15.0, -35.0, 0.0);
   writeBitmapString((void*)font, "Random color");
   
   glRasterPos3f(15.0, -40.0, 0.0);
   writeBitmapString((void*)font, "Color  r:       g:       b:");
   
}

// Drawing routine.
void drawScene(void)
{
    // Light position vectors.	
    float lightPos0[] = { 1.0, 1.0, 0.0, 0.0 };
    
    // Material property vectors.
    float matSpec[] = { 0.0, 0.0, 0.0, 1.0 };
    float matShine[] = { 50.0 };
    float matEmission[] = {0.0, 0.0, 0.0, 1.0};
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glDisable(GL_LIGHTING);  
    
    drawGUI();
    
    glColor3f(0.0, 0.0, 0.0);

    for (int i = 0; i < slides.size(); i++)
    {
        glBegin(GL_LINES);
        glVertex3f(slides[i].linex, slides[i].liney, 0.0);
        glVertex3f(slides[i].linex, slides[i].liney - lineLength, 0.0);
        glEnd();

        glPushMatrix();

        if(slides[i].sliding){
                slides[i].y = mousey;
                if(slides[i].y > slides[i].liney){
                    slides[i].y = slides[i].liney;
                }
                if(slides[i].y < slides[i].liney - lineLength){
                    slides[i].y = slides[i].liney - lineLength;
                }
        }

        glBegin(GL_POLYGON);
        glVertex3f(slides[i].linex - lineLength / 2.0, slides[i].y, 0.0);
        glVertex3f(slides[i].linex - lineLength / 2.0, slides[i].y - lineLength / 5.0, 0.0);
        glVertex3f(slides[i].linex + lineLength / 2.0, slides[i].y - lineLength / 5.0, 0.0);
        glVertex3f(slides[i].linex + lineLength / 2.0, slides[i].y, 0.0);
        glEnd();
        glPopMatrix();
    }
    
    displacement.x = ((slides[0].y - slides[0].liney) + lineLength / 2) / 50;
    displacement.y = ((slides[1].y - slides[1].liney) + lineLength / 2) / 50;
    displacement.z = ((slides[2].y - slides[2].liney) + lineLength / 2) / 50;
    
    randDisplacementMin.x = ((slides[3].y - slides[3].liney) + lineLength / 2) / 50;
    randDisplacementMin.y = ((slides[4].y - slides[4].liney) + lineLength / 2) / 50;
    randDisplacementMin.z = ((slides[5].y - slides[5].liney) + lineLength / 2) / 50;
    
    randDisplacementMax.x = ((slides[6].y - slides[6].liney) + lineLength / 2) / 50;
    randDisplacementMax.y = ((slides[7].y - slides[7].liney) + lineLength / 2) / 50;
    randDisplacementMax.z = ((slides[8].y - slides[8].liney) + lineLength / 2) / 50;
    
    redrawTime = (slides[9].liney - slides[9].y) * 100 + 50;
    
    lifeTime = (slides[10].liney - slides[10].y) * 1000 + 100;
    
    particleRadius = (slides[11].liney - slides[11].y) + 1;
    
    if (slides[12].liney - slides[12].y > lineLength / 2)
    {
        randomColor = false;
    }
    else
    {
        randomColor = true;
    }
    
    particleColor[0] = (slides[13].liney - slides[13].y) / 3;
    particleColor[1] = (slides[14].liney - slides[14].y) / 3;
    particleColor[2] = (slides[15].liney - slides[15].y) / 3;
   
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
        default:
            break;
    }
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

void processMouseActiveMotion(int x, int y)
{
    x -= 500;
    y = -(y - 500);
    float newX = x / 10.0;
    float newY = y / 10.0;
    
    for(int i = 0; i < slides.size(); i++){
            if(newX <= slides[i].linex + lineLength / 2.0 &&
                    newX >= slides[i].linex - lineLength / 2.0 && 
                    newY >= slides[i].y - lineLength / 5.0 && newY <= slides[i].y){
                    slides[i].sliding = true;
            }else if(newY < slides[i].y - 11.0 || newY > slides[i].y + 5){
                    slides[i].sliding = false;
            }
    }
    mousex = newX;
    mousey = newY;
}

void processMouse(int button, int state, int x, int y){
    if(state == GLUT_UP){
        for (int i = 0; i < slides.size(); i++)
        {
            slides[i].sliding = false;
        }
    }
}

//Draw our sliders
void drawSliders()
{
    // displacement
    newSlider(-26, -33);
    newSlider(-19, -33);
    newSlider(-12, -33);
    
    // random displacement min
    newSlider(-26, -38.5);
    newSlider(-19, -38.5);
    newSlider(-12, -38.5);
    
    // random displacement max
    newSlider(-26, -43);
    newSlider(-19, -43);
    newSlider(-12, -43);
    
    // redraw time
    newSlider(8, -33);
    
    // life time
    newSlider(8, -38.5);
    
    //particle size
    newSlider(8, -43);
    
    // random color
    newSlider(27, -33);
    
    // color (r,g,b,a)
    newSlider(25, -38.5);
    newSlider(32, -38.5);
    newSlider(39, -38.5);
}

// Main routine.
int main(int argc, char **argv) 
{
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
    glutMotionFunc(processMouseActiveMotion);
    glutMouseFunc(processMouse);
    drawSliders();
    glutMainLoop(); 

    return 0;
}


