#include "glFenetre.h"
#include "players.hpp"
#include "musique.hpp"

#include <cassert>
#include <vector>
#include <iostream>

#include <cstdlib>
#include <cstdint>
#include <unistd.h> // usleep
#include <ctime>
#include <string>

// Threads 
#include <boost/thread.hpp>
// Net
#include <boost/asio.hpp>

using boost::asio::ip::udp;
const int PORT = 1313;


void msleep(int ms){usleep(ms*1000);}

// Global variables
std::string HOST_IP;
bool keep_running=true;

Tetris GAME(12,16,0,1,1,3);
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
  if (!GAME.isover()) glutTimerFunc(GAME.fallMillis(), gameTimer, 0);
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
 
// Callback handlers for special-key event 
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

void task_glut()
{
  // Initialise and create window
  glutInitWindowSize(WIN.width, WIN.height);
  glutInitWindowPosition(WIN.windowPosX, WIN.windowPosY);
  glutCreateWindow(WIN.title);                           
  if (WIN.fullScreenMode) glutFullScreen();
  GAME.bounds=WIN.winBounds();     

  // Register callback handler for ...
  glutReshapeFunc(reshape);          // ... window re-shape
  glutDisplayFunc(display);          // ... window re-paint
  glutSpecialFunc(specialKeysPress); // ... special-key down event
  glutSpecialUpFunc(specialKeysUp);  // ... special-key up event
  glutKeyboardFunc(keyboard);        // ... ascii key event

  glutTimerFunc(0, displayTimer, 0);   // First timer call immediately

  glutMainLoop();               // Enter event-processing loop
  keep_running=false;
}

void task_net()
{

  std::vector<float> send_buf;
  std::vector<float> recv_buf;
  try
    {
      // Initialisation
      boost::asio::io_service io_service;
      boost::asio::ip::address ip_recv = boost::asio::ip::address::from_string(HOST_IP);
      udp::endpoint receiver_endpoint(ip_recv,PORT);
      udp::socket socket(io_service);
      socket.open(udp::v4());
      std::cout<<"connect to "<<ip_recv.to_string()<<std::endl;

      while(keep_running)
	{
	  GAME.get_data(send_buf);
	  socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

	  udp::endpoint sender_endpoint;
	  size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
	  GAME.set_data(recv_buf);

	  msleep(100);
	}
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
}



// Main function: GLUT runs as a console application starting at main()
int main(int argc, char** argv) {

  srand(time(NULL));
  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
  glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black

  if (argc < 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }
  else
    {
      HOST_IP=argv[1];
    }

 // Create and run threads
  boost::thread thread_1 = boost::thread(task_glut);
  boost::thread thread_2 = boost::thread(task_net);
  
  //do other stuff
  MUSIQUE.load(0);
  MUSIQUE.play();

  // Join threads
  thread_1.join();
  thread_2.join();
  


  return 0;
}
