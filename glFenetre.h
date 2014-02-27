#pragma once

#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <iostream>
#include <cassert>
#include <vector>



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

    std::vector<float> s(4,0.0);
    this->reshape(640,480,s);
  }

  void reshape(int w, int h,std::vector<float> s) 
  {   
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
	xLeft   = s[0] * aspect;
	xRight  = s[1] * aspect;
	yBottom = s[2];
	yTop    = s[3];
      } 
    else 
      {
	xLeft   = s[0];
	xRight  = s[1];
	yBottom = s[2] / aspect;
	yTop    = s[3] / aspect;
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




 


