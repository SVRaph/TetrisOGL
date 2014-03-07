
// g++ gl2d.cpp -lGL -lGLU -lglut


#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>     // Needed for sin, cos
#include <iostream>
#define PI 3.14159265f

#include <cassert>
#include <vector>
#include <cstdlib>

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

    fullScreenMode = true;
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

};


class Pong
{
public:
  float bx,by; // ball position
  float vx,vy;
  float p[2]; // players positions
  int score[2];
  
  float br; // ball radius
  float margin;
  float thickness;
  float size[2];

  // up/down up/down
  std::vector<bool> keypress;

  Pong()
  {
    br=0.1; // ball radius
    margin = 0.1;
    thickness=0.02;

    size[0]=0.2;
    size[1]=0.2;

    score[0]=0;
    score[1]=0;

    bx=0.0f;
    by=0.0f;
    vx=0.02f;
    vy=0.001f;

    keypress.resize(4,false);
  }

  void gldisplay(glFenetre& fen)
  {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix

    // Ball
    glLoadIdentity();              // Reset model-view matrix
    glTranslatef(bx, by, 0.0f);  // Translate to (xPos, yPos)
    // Use triangular segments to form a circle
    glBegin(GL_TRIANGLE_FAN);
    float sum=5.0f+(float)score[0]+(float)score[1];
    glColor3f(5.0f/sum, (float)score[0]/sum , (float)score[1]/sum );  // Red
    glVertex2f(0.0f, 0.0f);       // Center of circle
    int numSegments = 100;
    GLfloat angle;
    for (int i = 0; i <= numSegments; i++)  // Last vertex same as first vertex
      {
	angle = i * 2.0f * PI / numSegments; 
	glVertex2f(cos(angle) * br, sin(angle) * br);
      }
    glEnd();

    // Players
    glLoadIdentity();
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);  
    glVertex2f(fen.xLeft+margin,p[0]);
    glVertex2f(fen.xLeft+margin-thickness,p[0]);
    glVertex2f(fen.xLeft+margin-thickness,p[0]+size[0]);
    glVertex2f(fen.xLeft+margin,p[0]+size[0]);

    glColor3f(0.0f, 0.0f, 1.0f); 
    glVertex2f(fen.xRight-margin,p[1]);
    glVertex2f(fen.xRight-margin+thickness,p[1]);
    glVertex2f(fen.xRight-margin+thickness,p[1]+size[1]);
    glVertex2f(fen.xRight-margin,p[1]+size[1]);
    glEnd();

  }

  void keyboard()
  {
    float speed=0.05;
    if (keypress[0])
      {
	p[0]+=speed;
	if (p[0]>(1.0-size[0])) p[0]=(1.0-size[0]);
      }
    if (keypress[1])
      {
	p[0]-=speed;
	if (p[0]<-1.0) p[0]=-1.0;
      }

    if (keypress[2])
      {
	p[1]+=speed;
	if (p[1]>(1.0-size[1])) p[1]=(1.0-size[1]);
      }
    if (keypress[3])
      {
	p[1]-=speed;
	if (p[1]<-1.0) p[1]=-1.0;
      }
  }

  void update(glFenetre& fen)
  {
    bx+=vx;
    by+=vy;
    // Check if the ball exceeds the edges
    if (bx+br > fen.xRight) 
      {
	bx = fen.xRight-br;
	vx = -vx;
	score[0]++;
	std::cout<<score[0]<<" - "<<score[1]<<std::endl;
      } 
    else if (bx-br < fen.xLeft) 
      {
	bx = fen.xLeft+br;
	vx= -vx;
	score[1]++;
	std::cout<<score[0]<<" - "<<score[1]<<std::endl;
      }
    else if (by+br > fen.yTop) 
      {
	by = fen.yTop-br;
	vy = -vy;
      } 
    else if (by-br < fen.yBottom) 
      {
	by = fen.yBottom+br;
	vy= -vy;
      }
    // contact : à améliorer
    else if ( ((by+br)>(p[0])) && ((by-br)<(p[0]+size[0])) && (bx-br < fen.xLeft+margin) )
      {
	float nv=(vx*vx+vy*vy);
	vx = fabs(vx);
	if (by<p[0]) 
	  {
	    vy=vx*(by-p[0])/(bx-fen.xLeft-margin);
	  }
	else if (by>p[0]+size[0]) 
	  {
	    vy=vx*(by-p[0]-size[0])/(bx-fen.xLeft-margin);
	  }
	float nv2=vx*vx+vy*vy;
	vx*=1.05*sqrt(nv/nv2);
	vy*=1.05*sqrt(nv/nv2);
      }
    else if ( ((by+br)>(p[1])) && ((by-br)<(p[1]+size[1])) && (bx+br > fen.xRight-margin) )
      {
	float nv=(vx*vx+vy*vy);
	vx = -fabs(vx);
	if (by<p[1]) 
	  {
	    vy=vx*(by-p[1])/(bx-fen.xRight+margin);
	  }
	else if (by>p[1]+size[1]) 
	  {
	    vy=vx*(by-p[1]-size[1])/(bx-fen.xRight+margin);
	  }
	float nv2=vx*vx+vy*vy;
	vx*=1.05*sqrt(nv/nv2);
	vy*=1.05*sqrt(nv/nv2); 
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
 
  GAME.gldisplay(WIN);

  glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
 
}
 
// Call back when the windows is re-sized 
void reshape(GLsizei w, GLsizei h) 
{
  WIN.reshape(w,h);
}
 
// Called back when the timer expired 
void displayTimer(int value) {
  GAME.update(WIN);
  GAME.keyboard();
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
  }
}
/*
// Callback handler for special-key event 
void specialKeysPress(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_F1: // F1: Toggle between full-screen and windowed mode
    WIN.fullscreenToggle();
    break;
  case GLUT_KEY_RIGHT:
    break;
  case GLUT_KEY_LEFT:  
    break;
  case GLUT_KEY_UP:  
    GAME.p[0]+=0.1;
    if (GAME.p[0]>(1.0-GAME.size[0])) GAME.p[0]=(1.0-GAME.size[0]);
    break;
  case GLUT_KEY_DOWN: 
    GAME.p[0]-=0.1;
    if (GAME.p[0]<-1.0) GAME.p[0]=-1.0;
    break;
  case GLUT_KEY_PAGE_UP:
    GAME.p[1]+=0.1;
    if (GAME.p[1]>(1.0-GAME.size[1])) GAME.p[1]=(1.0-GAME.size[1]);
    break;
  case GLUT_KEY_PAGE_DOWN:
    GAME.p[1]-=0.1;
    if (GAME.p[1]<-1.0) GAME.p[1]=-1.0;
    break;
  }
}
*/
// Callback handler for special-key event 
void specialKeysPress(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_F1: // F1: Toggle between full-screen and windowed mode
    WIN.fullscreenToggle();
    break;
  case GLUT_KEY_RIGHT:
    break;
  case GLUT_KEY_LEFT:  
    break;
  case GLUT_KEY_UP: 
    GAME.keypress[0]=true;
    break;
  case GLUT_KEY_DOWN:
    GAME.keypress[1]=true;
    break;
  case GLUT_KEY_PAGE_UP:
    GAME.keypress[2]=true;
    break;
  case GLUT_KEY_PAGE_DOWN:
    GAME.keypress[3]=true;
    break;
  }
  GAME.keyboard();
}



// Callback handler for special-key event 
void specialKeysUp(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP: 
    GAME.keypress[0]=false;
    break;
  case GLUT_KEY_DOWN:
    GAME.keypress[1]=false;
    break;
  case GLUT_KEY_PAGE_UP:
    GAME.keypress[2]=false;
    break;
  case GLUT_KEY_PAGE_DOWN:
    GAME.keypress[3]=false;
    break;
  }
}



/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Enable double buffered mode
  glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
  glutInitWindowSize(WIN.width, WIN.height);  // Initial window width and height
  glutInitWindowPosition(WIN.windowPosX, WIN.windowPosY); // Initial window top-left corner (x, y)
  glutCreateWindow(WIN.title);      // Create window with given title
  glutFullScreen();             // Put into full screen

  glutDisplayFunc(display);     // Register callback handler for window re-paint
  glutReshapeFunc(reshape);     // Register callback handler for window re-shape
  glutSpecialFunc(specialKeysPress); // Register callback handler for special-key event
  glutSpecialUpFunc(specialKeysUp); // Register callback handler for special-key event
  glutKeyboardFunc(keyboard);   // Register callback handler for special-key event

  glutTimerFunc(0, displayTimer, 0);   // First timer call immediately

  glutMainLoop();               // Enter event-processing loop

  
  return 0;
}


