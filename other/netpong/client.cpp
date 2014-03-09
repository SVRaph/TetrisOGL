#include "netpong.hpp"
// g++ client.cpp -o pong_client -lboost_system -lboost_thread -lpthread -lglut -lGLU -lGL

std::string HOST_IP;

bool keep_running=true;
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
void reshape(GLsizei w, GLsizei h) 
{
  WIN.reshape(w,h);
  GAME.bounds=WIN.winBounds();
}
 
// Called back when the timer expired 
void displayTimer(int value) {
  GAME.update();
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


// GLUT runs as a console program
void task_glut()
{

  glutInitWindowSize(WIN.width, WIN.height);  // Initial window width and height
  glutInitWindowPosition(WIN.windowPosX, WIN.windowPosY); // Initial window top-left corner
  glutCreateWindow(WIN.title);      // Create window with given title
  //glutFullScreen();             // Put into full screen

  // Register callback handler for ...
  glutReshapeFunc(reshape);          // ... window re-shape
  glutDisplayFunc(display);          // ... window re-paint
  glutSpecialFunc(specialKeysPress); // ... special-key down event
  glutSpecialUpFunc(specialKeysUp);  // ... special-key up event
  glutKeyboardFunc(keyboard);        // ... ascii key event

  GAME.bounds=WIN.winBounds();
  glutTimerFunc(0, displayTimer, 0);   // First timer call immediately

  glutMainLoop();               // Enter event-processing loop
  keep_running=false;
}
void task_net()
{

  std::vector<float> send_buf(2);
  std::vector<float> recv_buf(4);
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
	  GAME.client_get_data(send_buf,0);
	  socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

	  udp::endpoint sender_endpoint;
	  size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
	  GAME.client_set_data(recv_buf);
	  //std::cout.write(recv_buf.data(), len);

	  msleep(100);
	}
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
}

// Main function: GLUT runs as a console program
int main(int argc, char** argv) 
{
  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Enable double buffered mode
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

  // Join threads
  thread_1.join();
  thread_2.join();
  
  return 0;
}


