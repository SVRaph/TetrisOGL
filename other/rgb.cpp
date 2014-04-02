
// g++ rgb.cpp -lGL -lGLU -lglut


#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>    
#include <iostream>
#define PI 3.14159265f

#include <cassert>
#include <vector>
#include <cstdlib>
#include <time.h>

class Color
{
public:
  float r;
  float g;
  float b;
  
 void black()
  {
    r=0;
    g=0;
    b=0;
  }
  void random()
  {
    r=(float)rand() / (float)RAND_MAX;
    g=(float)rand() / (float)RAND_MAX;
    b=(float)rand() / (float)RAND_MAX;
  }
  float sum(){return r+g+b;}
  float mean(){return r+g+b/3.0f;}
  float norm2(){return r*r+g*g+b*b;}
  void translate(float x)
  {
    r+=x;
    g+=x;
    b+=x;
  }
  Color operator-(Color c)
  {
    Color d;
    d.r=r-c.r;
    d.g=g-c.g;
    d.b=b-c.b; 
    return d;
  }
};

float distColor(Color a,Color b)
{
  float lambda=0.2f;

  float am=a.mean();
  float bm=b.mean();
  float d2m=(am-bm)*(am-bm);
  a.translate(-am);
  b.translate(-bm);
  Color c=a-b;
  return c.norm2()+lambda*d2m;
}

class glFenetre
{
public:
  char* title;  // Windowed mode's title
  int width;     // Windowed mode's width
  int height;     // Windowed mode's height
  int windowPosX;      // Windowed mode's top-left corner x
  int windowPosY;      // Windowed mode's top-left corner y

  // Projection clipping area
  GLdouble xLeft, xRight, yBottom, yTop;

  bool fullScreenMode; // Full-screen or windowed mode?

  int refreshMillis;      // Refresh period in milliseconds
  
  glFenetre()
  {
    title = (char*)("Full-Screen & Windowed Mode");
    width = 640; 
    height = 480;
    windowPosX   = 50; 
    windowPosY   = 50;  

    fullScreenMode = false;
    refreshMillis = 30;  

    this->reshape(640,480);
  }

  void reshape(int w, int h) 
  {
    //width=w;
    //height=h;

    // Compute aspect ratio of the new window
    if (h == 0) h = 1;                // To prevent divide by 0
    float aspect = (float)w / (float)h;

    // Set the viewport to cover the new window
    glViewport(0, 0, w, h);
 
    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();           
    if (w >= h) 
      {
	xLeft   = -1.0 * aspect;
	xRight  = 1.0 * aspect;
	yBottom = -1.0;
	yTop    = 1.0;
      } 
    else 
      {
	xLeft   = -1.0;
	xRight  = 1.0;
	yBottom = -1.0 / aspect;
	yTop    = 1.0 / aspect;
      }
    gluOrtho2D(xLeft, xRight, yBottom, yTop);
  }

  void fullscreenToggle()
  {
    fullScreenMode = !fullScreenMode;  
    if (fullScreenMode) // Full-screen mode
      {              
	windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
	windowPosY   = glutGet(GLUT_WINDOW_Y);
	width  = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);
	glutFullScreen();                  
      }     
    else      // Windowed mode
      {                                      
	glutReshapeWindow(width, height); // Switch into windowed mode
	glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
      }
  }

  std::vector<float> winBounds()
  {
    std::vector<float> v(4,0.0f);
    v[0]=(float)xLeft;
    v[1]=(float)xRight;
    v[2]=(float)yBottom;
    v[3]=(float)yTop;
    return v;
  } 
};



void drawCircle(float x, float y, float br, float r, float g, float b, int numSegments=100)
{   
  // Reset model-view matrix
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(x, y, 0.0f); 

  // Use triangular segments to form a circle
  glBegin(GL_TRIANGLE_FAN);
  glColor3f(r, g, b);
  glVertex2f(0.0f, 0.0f);       // Center of circle
  GLfloat angle;
  for (int i = 0; i <= numSegments; i++)  // Last vertex same as first vertex
    {
      angle = i * 2.0f * PI / numSegments; 
      glVertex2f(cos(angle) * br, sin(angle) * br);
    }
  glEnd();

  glPopMatrix();
}

class Pong
{
public:
  static const int n=10;

  // samples positions
  float x[2*n];
  float y[2*n];
 
  // samples colors
  Color c[2*n];
  
  // radius
  float br;

  Pong()
  {
    br=0.1;

    for(int i=0;i<n;i++)
      {
	x[i]=(float)(i-n/2) / (float)(n/2);
	x[i+n]=x[i];

	y[i]=+0.6f;
	y[i+n]=-0.6f;
      }

    update();
  }

  void gldisplay()
  {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
    glLoadIdentity();              // Reset model-view matrix

    for(int i=0;i<(2*n);i++)
      {
	drawCircle(x[i],y[i],br,c[i].r,c[i].g,c[i].b);
      }
  }

  void keyboard()
  {   
  }

  void update2()
  {
    for(int i=0;i<n;i++)
      {
	c[i].random();
	c[i+n].black();
      }
    c[n]=c[0];
    int jmin=1;
    for(int j=2;j<n;j++)
      {
	if (distColor(c[0],c[j]) < distColor(c[0],c[jmin]))
	  jmin=j;
      }
    c[n]=c[0];
    c[jmin+n]=c[jmin];
  }
 void update()
  {
    for(int i=0;i<n;i++)
      c[i].random();
    c[n]=c[0];

    std::vector< std::vector<float> > D(n, std::vector<float>(n,0.0f) );
    for(int i=0;i<n;i++)
      for(int j=0;j<n;j++)
	D[i][j]=distColor(c[i],c[j]);

    std::vector<bool> vused(n,false);
    c[n]=c[0];
    vused[0]=true;
    int i0=0; // indice du précédent
    for(int i=n+1;i<2*n;i++)
      {
	int jmin=0;
	float dmin=100.0f; // %inf
	for(int j=1;j<n;j++)
	  if(!vused[j] && D[i0][j]<dmin)
	    {
	      jmin=j;
	      dmin=D[i0][j];
	    }
	assert(jmin>0);
	c[i]=c[jmin];
	vused[jmin]=true;
	i0=jmin;
      }
  }
};

glFenetre WIN;
Pong GAME;

// Callback handler for window re-paint event 
void display() {
  glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
  glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
  glLoadIdentity();              // Reset model-view matrix
 
  GAME.gldisplay();

  glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
 
}
// Call back when the windows is re-sized 
void reshape(GLsizei w, GLsizei h) {
  WIN.reshape(w,h);
}
 
// Called back when the timer expired 
void displayTimer(int value) {
  glutPostRedisplay();    // Post a paint request to activate display()
  glutTimerFunc(WIN.refreshMillis, displayTimer, 0); // subsequent timer call at milliseconds
}

// Callback handler for normal-key event
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:     // ESC key
    glutDestroyWindow(glutGetWindow());
    exit(0);
    break;
  case 32: // space key
    GAME.update();
    break;
  }
}
// Callback handler for special-key event 
void specialKeysPress(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_F1: // F1: Toggle full-screen mode
    WIN.fullscreenToggle();
    break;
  }
}

// Callback handler for special-key event 
void specialKeysUp(int key, int x, int y) {
}



/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) 
{
  srand (time(NULL));


  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Enable double buffered mode
  glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
  glutInitWindowSize(WIN.width, WIN.height);  // Initial window width and height
  glutInitWindowPosition(WIN.windowPosX, WIN.windowPosY); // Initial window top-left corner (x, y)
  glutCreateWindow(WIN.title);      // Create window with given title
  //glutFullScreen();             // Put into full screen

  // Register callback handler for ...
  glutReshapeFunc(reshape);          // ... window re-shape
  glutDisplayFunc(display);          // ... window re-paint
  glutSpecialFunc(specialKeysPress); // ... special-key down event
  glutSpecialUpFunc(specialKeysUp);  // ... special-key up event
  glutKeyboardFunc(keyboard);        // ... ascii key event

  glutTimerFunc(0, displayTimer, 0);   // First timer call immediately

  glutMainLoop();               // Enter event-processing loop

  
  return 0;
}


