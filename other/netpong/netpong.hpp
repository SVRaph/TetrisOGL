#include <GL/glut.h>  
#include <cmath>    
#include <iostream>
#define PI 3.14159265f

#include <cassert>
#include <vector>
#include <cstdlib>
#include <unistd.h> // usleep

// Threads 
#include <boost/thread.hpp>

// Net
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
const int PORT = 1313;


void msleep(int ms){usleep(ms*1000);}

const int WIN_WIDTH=720;
const int WIN_HEIGHT=480;
const float RATIO =  (float)WIN_WIDTH / (float)WIN_HEIGHT;

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
    width = WIN_WIDTH; 
    height = WIN_HEIGHT;
    windowPosX   = 50; 
    windowPosY   = 50;  

    fullScreenMode = false;
    refreshMillis = 30;  

    this->reshape(width,height);
  }

  void reshape(int w, int h) 
  {
    // Compute aspect ratio of the new window
    if (h == 0) h = 1;                // To prevent divide by 0
    float aspect = (float)w / (float)h;
    if (aspect!=RATIO) return;

    // Set the viewport to cover the new window
    glViewport(0, 0, w, h);
 
    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();           
    xLeft   = -1.0 * aspect;
    xRight  = 1.0 * aspect;
    yBottom = -1.0;
    yTop    = 1.0; 
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


class Pong
{
public:
  float bx,by; // ball position
  float vx,vy; // ball speed
  float p[2]; // players positions
  int score[2];
  
  float br; // ball radius
  float margin;
  float thickness;
  float size[2];

  // Bounds: xRight, xLeft, yBottom, yTop
  std::vector<float> bounds;
  // J1:up/down J2:up/down
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
    vx=2.0f;
    vy=0.1f;

    keypress.resize(4,false);
  }

  void gldisplay()
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
    glVertex2f(bounds[0]+margin,p[0]);
    glVertex2f(bounds[0]+margin-thickness,p[0]);
    glVertex2f(bounds[0]+margin-thickness,p[0]+size[0]);
    glVertex2f(bounds[0]+margin,p[0]+size[0]);

    glColor3f(0.0f, 0.0f, 1.0f); 
    glVertex2f(bounds[1]-margin,p[1]);
    glVertex2f(bounds[1]-margin+thickness,p[1]);
    glVertex2f(bounds[1]-margin+thickness,p[1]+size[1]);
    glVertex2f(bounds[1]-margin,p[1]+size[1]);
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

  void update(float dt=1.0f)
  {
    bx+=dt*vx;
    by+=dt*vy;
    // Check if the ball exceeds the edges
    if (bx+br > bounds[1]) 
      {
	bx = bounds[1]-br;
	vx = -vx;
	score[0]++;
	std::cout<<score[0]<<" - "<<score[1]<<std::endl;
      } 
    else if (bx-br < bounds[0]) 
      {
	bx = bounds[0]+br;
	vx= -vx;
	score[1]++;
	std::cout<<score[0]<<" - "<<score[1]<<std::endl;
      }
    else if (by+br > bounds[3]) 
      {
	by = bounds[3]-br;
	vy = -vy;
      } 
    else if (by-br < bounds[2]) 
      {
	by = bounds[2]+br;
	vy= -vy;
      }
    // contact : à améliorer
    else if ( ((by+br)>(p[0])) && ((by-br)<(p[0]+size[0])) && (bx-br < bounds[0]+margin) )
      {
	float nv=(vx*vx+vy*vy);
	vx = fabs(vx);
	if (by<p[0]) 
	  {
	    vy=vx*(by-p[0])/(bx-bounds[0]-margin);
	  }
	else if (by>p[0]+size[0]) 
	  {
	    vy=vx*(by-p[0]-size[0])/(bx-bounds[0]-margin);
	  }
	float nv2=vx*vx+vy*vy;
	vx*=1.05*sqrt(nv/nv2);
	vy*=1.05*sqrt(nv/nv2);
      }
    else if ( ((by+br)>(p[1])) && ((by-br)<(p[1]+size[1])) && (bx+br > bounds[1]-margin) )
      {
	float nv=(vx*vx+vy*vy);
	vx = -fabs(vx);
	if (by<p[1]) 
	  {
	    vy=vx*(by-p[1])/(bx-bounds[1]+margin);
	  }
	else if (by>p[1]+size[1]) 
	  {
	    vy=vx*(by-p[1]-size[1])/(bx-bounds[1]+margin);
	  }
	float nv2=vx*vx+vy*vy;
	vx*=1.05*sqrt(nv/nv2);
	vy*=1.05*sqrt(nv/nv2); 
      }
  }

  void serveur_set_data(const std::vector<float>& recv_buf)
  {
    if (recv_buf.size()<2)
      std::cout<<"no data receive"<<std::endl;
    else
      p[(int)recv_buf[0]]=recv_buf[1];
  }
  void serveur_get_data(std::vector<float>& send_buf)
  {
    send_buf.resize(4);
    send_buf[0]=p[0];
    send_buf[1]=p[1];
    send_buf[2]=bx;
    send_buf[3]=by;
  }
  void client_get_data(std::vector<float>& send_buf,int jou=0)
  {
    send_buf.resize(2);
    send_buf[0]=(float)jou;
    send_buf[1]=p[jou];
  }
  void client_set_data(const std::vector<float>& recv_buf)
  {
    // on ignore le joueur pour l'instant
    p[0]=recv_buf[0];
    p[1]=recv_buf[1];
    // si l'écart et inférieur à 1e-2s on ne touche à rien
    //if ( ((recv_buf[2]-bx)*(recv_buf[2]-bx)+(recv_buf[3]-by)*(recv_buf[3]-by)) > ((vx*vx+vy*vy)*1e-4) )
      {
	bx  =recv_buf[2];
	by  =recv_buf[3];
      }
  }

};


