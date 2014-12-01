#include "glFenetre.hpp"
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

// Arguments
#include "arguments.hpp"


using boost::asio::ip::udp;

std::string HOST_IP;

void msleep(int ms){usleep(ms*1000);}

// Global parameters
bool keep_running=false;

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
  if (keep_running)
    {
      glutPostRedisplay();    // Post a paint request to activate display()
      glutTimerFunc(WIN.refreshMillis, displayTimer, 0); // subsequent timer call
    }
  else
    {
      glutDestroyWindow(glutGetWindow());
    }
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
      keep_running=false;
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
    keep_running=false;
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

  // Register callback handler for ...
  glutReshapeFunc(reshape);          // ... window re-shape
  glutDisplayFunc(display);          // ... window re-paint
  glutSpecialFunc(specialKeys);      // ... special-key down event
  glutSpecialUpFunc(specialUpKeys);  // ... special-key up event
  glutKeyboardFunc(keyboard);        // ... ascii key event

  sleep(1);
  while(!keep_running) 
    {
      std::cout<<"waiting server instruction..."<<std::endl;
      sleep(1);
    }
  glutTimerFunc(0, displayTimer, 0);   // First timer call immediately
  glutTimerFunc(0, gameTimer, 0);
  glutTimerFunc(0, moveTimer, 0);

  glutMainLoop();               // Enter event-processing loop

}

void task_net(Parameters& P)
{
  std::vector<uint32_t> send_buf(BUFFER_LEN);
  std::vector<uint32_t> recv_buf;
  try
    {
      // Initialisation
      boost::asio::io_service io_service;
      boost::asio::ip::address ip_recv = boost::asio::ip::address::from_string(HOST_IP);
      udp::endpoint receiver_endpoint(ip_recv,PORT);
      udp::endpoint sender_endpoint;
      udp::socket socket(io_service);
      socket.open(udp::v4());
      std::cout<<"connect to "<<ip_recv.to_string()<<std::endl;
      
      // Premiere connexion
      send_buf[0]=1;
      recv_buf.resize(4);
      socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
      socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
      
      GAME.reinit(recv_buf[2],recv_buf[3],P.nhu,P.nia,recv_buf[1]-1);
      keep_running=true;

      // Boucle principale
      recv_buf.resize(BUFFER_LEN);
      while(keep_running)
	{
	  GAME.get_data(send_buf); // ne redimensionne pas
	  socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
	  socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
	  GAME.set_data(recv_buf);
	  msleep(100);
	}

      // Derniere connexion
      send_buf[0]=3;
      recv_buf.resize(4);
      socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
      socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
      std::cout<<"Déconnexion\n";
      msleep(100);
    }
  catch (std::exception& e)
    {
      std::cerr << "Exception net task: "<<e.what() << std::endl;
    }
}



// Main function: GLUT runs as a console application starting at main()
int main(int argc, char** argv) {

  MUSIQUE.load(0);

  Parameters P(argc,argv);
  if (P.help) return 0;
  if (P.music) MUSIQUE.play();

  srand(time(NULL));
  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
  glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black

  if (P.nnt==0) // Stand-alone
    {
      GAME.reinit(P.l,P.h,P.nhu,P.nia,P.nnt,P.lvl);
      keep_running=true;
      task_glut();
    }
  else
    {
      if (P.nhu+P.nia != 1)
	{
	  std::cout<<"Le nombre de joueurs locaux d'une partie en réseaux et limité à 1 par client"<<std::endl;
	  return 0;
	}
      HOST_IP=P.host_ip;


      // Create and run threads
      boost::thread thread_1 = boost::thread(task_glut);
      boost::thread thread_2 = boost::thread(task_net,P);

      // do other stuff
      // ...

      // Join threads
      thread_1.join();
      thread_2.join();
    }


  return 0;
}
