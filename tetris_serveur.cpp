#include <cassert>
#include <vector>
#include <cstdint>
#include <iostream>

// Net
#include <boost/asio.hpp>

using boost::asio::ip::udp;
const int PORT = 1313;
const int BUFFER_LEN=3+1+12*16;

// la classe host représente 1 client
struct Host
{
  udp::endpoint remote_endpoint;
  std::vector<uint32_t> buffer;
};

// la classe serveur contient les données envoyées par tous les clients
class Server
{
  std::vector< Host > clients;
  int sx,sy;
public:
  Server(int w,int h): sx(w),sy(h){}
  int nb_clients(){return clients.size();}
  void set_data(const std::vector<uint32_t>& buf,const udp::endpoint& remote)
  {
    int n=buf[0];
    int len=3+n+n*sx*sy;
    if (nb_clients()==0)
      {
	sx=buf[1];
	sy=buf[2];
      }

    assert(sx==buf[1] && sy==buf[2] && len==buf.size());
    bool found=false;
    for (int c=0;c<nb_clients();c++)
      {
	if (remote==clients[c].remote_endpoint)
	  {
	    found=true;
	    clients[c].buffer=buf;
	  }
      }
    if(!found)
      {
	Host h;
	h.remote_endpoint=remote;
	h.buffer=buf;
	clients.push_back(h);
	std::cout<<"New client"<<std::endl;
      }
  }
  void get_data(std::vector<uint32_t>& buf,const udp::endpoint& remote)
  {
    if (nb_clients()<2)
      {
	buf[0]=1;
	buf[1]=sx;
	buf[2]=sy;
      }
    else if (remote==clients[0].remote_endpoint) 
      buf=clients[1].buffer;
    else if (remote==clients[1].remote_endpoint) 
      buf=clients[0].buffer;
    else
      {
	std::cout<<"More than 2 different clients: not implemented yet"<<std::endl;
	buf[0]=1;
	buf[1]=sx;
	buf[2]=sy;
      }
  }
};



// Run the server
int main(int argc, char** argv) 
{
  // Global variables
  Server SERVEUR(12,16);
  bool keep_running=true;

  std::vector<uint32_t> send_buf;
  std::vector<uint32_t> recv_buf;
  try
    {
      boost::asio::io_service io_service;
      udp::socket socket(io_service, udp::endpoint(udp::v4(), PORT));
  
      std::vector<uint32_t> send_buf(BUFFER_LEN);
      std::vector<uint32_t> recv_buf(BUFFER_LEN);
      udp::endpoint remote_endpoint;

      while(keep_running)
	{
	  boost::system::error_code error;
	  socket.receive_from(boost::asio::buffer(recv_buf),
			      remote_endpoint, 0, error);

	  if (error && error != boost::asio::error::message_size)
	    throw boost::system::system_error(error);

	  SERVEUR.set_data(recv_buf,remote_endpoint);
	  SERVEUR.get_data(send_buf,remote_endpoint);

	  boost::system::error_code ignored_error;
	  socket.send_to(boost::asio::buffer(send_buf),
			 remote_endpoint, 0, ignored_error);
	}
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }


  return 0;
}
