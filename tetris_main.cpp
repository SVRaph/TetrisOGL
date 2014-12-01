
// g++ gl2d.cpp -lGL -lGLU -lglut
// -- obsolete --

 
#include <iostream>
 
#include "glFenetre.hpp"
#include "players.hpp"
#include "musique.hpp"

#include "arguments.hpp"

// Global variables
Tetris GAME;
glFenetre WIN; 
Music MUSIQUE;

// Callback handler for window re-paint event 
void display() {
  glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
  glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
  glLoadIdentity();              // Reset model-view matrix
 
  GAME.gldisplay();

  glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
}
// Call back when the windows is re-sized 
void reshape(GLsizei width, GLsizei height) {
  WIN.reshape(width,height,GAME.winBounds());
}
 
// Called back when the timer expired
void displayTimer(int value) {
  glutPostRedisplay();    // Post a paint request to activate display()
  glutTimerFunc(WIN.refreshMillis, displayTimer, 0); // subsequent timer call
}
void gameTimer(int value) {
  // pour les mvt autos : IA et descente
  static int k=-1;
  k=(k+1)%7;
  GAME.command(true); // mouvement de l'IA
  if (k==0) GAME.update();
  if (!GAME.isover())
    glutTimerFunc(GAME.fallMillis(), gameTimer, 0);
  else 
    {
      std::cout<<"Enter an int to quit"<<std::endl;
      std::cin>>k;
      if (k==0)
	glutDestroyWindow(glutGetWindow());
      else
	{
	  GAME.reinit(12,16,0,1,0,7);
	  glutTimerFunc(0, gameTimer, 0);
	}
    }
}
void moveTimer(int jou) {
  // pour la répétition des touches
  // ne supporte ici qu'un joueur clavier
  GAME.command(false); // mvt humain
  if (GAME.iskeypressed()) glutTimerFunc(GAME.moveMillis(), moveTimer, jou);
}
 
// Callback handler for normal-key event
void keyboard(unsigned char key, int x, int y) {
  static bool pause=false;
  switch (key) {
    // ESC key
  case 27:    
    glutDestroyWindow(glutGetWindow());
    exit(0);
    break;
    // P key
  case 80:
  case 112:
    GAME.ispaused=!GAME.ispaused;
    break;
   // M key
  case 77:
  case 109:
    MUSIQUE.switchpause();
    break;
    // N key
  case 78:
  case 110:
    MUSIQUE.next();
    break;
  }
}
 
// Callback handler for special-key event 
void specialKeys(int key, int x, int y) {
  switch (key) 
    {
    case GLUT_KEY_F11:    // F11: Toggle between full-screen and windowed mode
      WIN.fullscreenToggle();
      break;
    case GLUT_KEY_PAGE_UP:  
      break;
    case GLUT_KEY_PAGE_DOWN:
      break;
    }
   GAME.keyboard(key,true);
   glutTimerFunc(GAME.moveMillis(), moveTimer, 0);
}
void specialUpKeys(int key,int x,int y){
  GAME.keyboard(key,false);
}

// Main function: GLUT runs as a console application starting at main()
int main(int argc, char** argv) {

  MUSIQUE.load(0);

  Parameters P(argc,argv);
  if (P.help) return 0;
  if (P.music) MUSIQUE.play();
  GAME.reinit(P.l,P.h,P.nhu,P.nia,P.nnt,P.lvl);

  srand(time(NULL));
  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
  glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black

  // Initialise and create window
  glutInitWindowSize(WIN.width, WIN.height);
  glutInitWindowPosition(WIN.windowPosX, WIN.windowPosY);
  glutCreateWindow(WIN.title);                           
  if (WIN.fullScreenMode) glutFullScreen(); 

  // Register callback handler for...
  glutDisplayFunc(display);     // ... window re-paint
  glutReshapeFunc(reshape);     // ... window re-shape
  glutSpecialFunc(specialKeys); // ... special-key event
  glutSpecialUpFunc(specialUpKeys); // ... special-key release event
  glutKeyboardFunc(keyboard);   // ... key event

  // Timer :  First timer call immediately
  glutTimerFunc(0, displayTimer, 0);
  glutTimerFunc(0, gameTimer, 0);
  glutTimerFunc(0, moveTimer, 0);

  glutMainLoop();               // Enter event-processing loop
  return 0;
}
