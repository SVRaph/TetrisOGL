
// g++ gl2d.cpp -lGL -lGLU -lglut


#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>     // Needed for sin, cos
#include <iostream>
 
#include "tetrominos.h"
#include "glFenetre.h"

// Global variables
Tetris GAME(12,16,1);
glFenetre WIN; 

// Callback handler for window re-paint event 
void display() {
  glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
  glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
  glLoadIdentity();              // Reset model-view matrix
 
  //DrawCube6(0,0,0,125,12,200);
  GAME.gldisplay();

  glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
 
}
 
// Call back when the windows is re-sized 
void reshape(GLsizei width, GLsizei height) 
{
  // Borne de la fenêtre
  WIN.reshape(width,height,GAME.winBounds());
}
 
/* Called back when the timer expired */
void displayTimer(int value) {
  glutPostRedisplay();    // Post a paint request to activate display()
  glutTimerFunc(WIN.refreshMillis, displayTimer, 0); // subsequent timer call at milliseconds
}
void gameTimer(int value) {
  GAME.update();
  glutTimerFunc(GAME.fallMillis, gameTimer, 0);
}

 
/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:     // ESC key
    glutDestroyWindow(glutGetWindow());
    exit(0);
    break;
  }
}
 
/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
  switch (key) 
    {
    case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
      WIN.fullscreenToggle();
      break;
    case GLUT_KEY_PAGE_UP:  
      break;
    case GLUT_KEY_PAGE_DOWN:
      break;
    }
  GAME.keyboard(key);

}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

  srand(time(NULL));
  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
  glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
  glutInitWindowSize(WIN.width, WIN.height);  // Initial window width and height
  glutInitWindowPosition(WIN.windowPosX, WIN.windowPosY); // Initial window top-left corner (x, y)
  glutCreateWindow(WIN.title);      // Create window with given title
  glutFullScreen();             // Put into full screen

  glutDisplayFunc(display);     // Register callback handler for window re-paint
  glutReshapeFunc(reshape);     // Register callback handler for window re-shape
  glutSpecialFunc(specialKeys); // Register callback handler for special-key event
  glutKeyboardFunc(keyboard);   // Register callback handler for special-key event

  glutTimerFunc(0, displayTimer, 0);   // First timer call immediately
  glutTimerFunc(0, gameTimer, 0);   // First timer call immediately

  glutMainLoop();               // Enter event-processing loop
  return 0;
}
