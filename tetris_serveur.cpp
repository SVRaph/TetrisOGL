
// g++ gl2d.cpp -lGL -lGLU -lglut

#include <iostream>
 
//#include "glFenetre.h"
#include "net_header.hpp"
#include "players.hpp"
//#include "musique.hpp"

// Global variables
bool keep_running=true;
Tetris GAME(12,16,0,1,1,3);



void task_game()
{
  // pour les mvt autos : IA et descente
  int k=-1;
  while (keep_running)
    {
      k=(k+1)%7;
      GAME.command(true); // mouvement de l'IA
      if (k==0) GAME.update();
      if (GAME.isover()) keep_running=false;
    }
}

void task_net()
{
  std::vector<uint32_t> send_buf;
  std::vector<uint32_t> recv_buf;
  try
    {
      boost::asio::io_service io_service;

      udp::socket socket(io_service, udp::endpoint(udp::v4(), PORT));

      while(keep_running)
	{
	  udp::endpoint remote_endpoint;
	  boost::system::error_code error;
	  socket.receive_from(boost::asio::buffer(recv_buf),
			      remote_endpoint, 0, error);

	  if (error && error != boost::asio::error::message_size)
	    throw boost::system::system_error(error);

	  GAME.set_data(recv_buf);
	  GAME.get_data(send_buf);

	  boost::system::error_code ignored_error;
	  socket.send_to(boost::asio::buffer(send_buf),
			 remote_endpoint, 0, ignored_error);
	}
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }

}


// Run the 2 threads
int main(int argc, char** argv) {

  srand(time(NULL));
  // Create and run threads
  boost::thread thread_1 = boost::thread(task_game);
  boost::thread thread_2 = boost::thread(task_net);

  // do other stuff
  while (keep_running) sleep(10);

  // Join threads
  thread_2.join();
  thread_1.join();

  return 0;
}
