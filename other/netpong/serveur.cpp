#include "netpong.hpp"
// g++ serveur.cpp -o pong_serveur -lboost_system -lboost_thread -lpthread

bool keep_running=true;
Pong GAME;

void task_game()
{
  GAME.bounds.resize(4,1.0f);
  GAME.bounds[0]=-1.0*RATIO;
  GAME.bounds[1]=+1.0*RATIO;
  GAME.bounds[2]=-1.0;
  GAME.bounds[3]=+1.0;
  while (keep_running)
    {
      GAME.update(0.08);
      msleep(80);
    }
}
void task_net()
{
  std::vector<float> send_buf(4);
  std::vector<float> recv_buf(2);
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

      GAME.serveur_set_data(recv_buf);
      GAME.serveur_get_data(send_buf);

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
int main(int argc, char** argv) 
{ 
  // Create and run threads
  boost::thread thread_1 = boost::thread(task_game);
  boost::thread thread_2 = boost::thread(task_net);
  
  // do other stuff
  while (keep_running) sleep(10);
  keep_running=false;

  // Join threads
  thread_2.join();
  thread_1.join();
  
  return 0;
}


