#include <cassert>
#include <vector>
#include <cstdint>
#include <iostream>

// Net
#include <boost/asio.hpp>

using boost::asio::ip::udp;

const int PORT = 1313;
const int BUFFER_LEN=194; // =1+1(1+12*16)


// la classe host représente 1 client
struct Host
{
  udp::endpoint remote_endpoint;
  std::vector<uint32_t> buffer;
};

// la classe serveur contient les données envoyées par tous les clients
class Server
{
  std::vector< bool > isFree;
  std::vector< Host > clients;
  std::vector< uint32_t > params;
public:
  Server(int n,int l,int h)
  {
    assert( (l*h) < BUFFER_LEN );
    params.resize(4);
    params[1]=n;
    params[2]=l;
    params[3]=h;
    clients.resize(n);
    for(int i=0;i<n;i++) clients[i].buffer.resize(len_data(),0);
    isFree.resize(n+1,true);
    std::cout<<"Serveur created. "<<n<<" clients maximum"<<std::endl;
  }
  int nb_clients(){return params[1];}
  int len_data(){return BUFFER_LEN;}
  int isKnown(const udp::endpoint& remote)
  {
    for (int c=0;c<nb_clients();c++)
      {
	if (!isFree[c] && clients[c].remote_endpoint==remote)
	  return c;
      }
    return -1;
  }
  int swap_data(const std::vector<uint32_t>& recv_buf,std::vector<uint32_t>& send_buf,const udp::endpoint& remote)
  {
    int flag=recv_buf[0];
    int found=isKnown(remote);
    // Nouvel arrivant
    if (found<0 && flag==1)
      {
	// Reception
	int c=0;
	while (!isFree[c]) c++;
	if (c>=nb_clients()) 
	  {
	    std::cout<<"Trop de clients"<<std::endl;
	    return -1;
	  }
	clients[c].remote_endpoint=remote;
	clients[c].buffer.resize(len_data(),0);
	isFree[c]=false;
	std::cout<<"New client: "<<c<<std::endl;

	// Envoi - len=4
	send_buf=params;
	send_buf[0]=flag;
      }
    // Mise à jour d'un client
    else if (found>=0 && flag==2)
      {
	// Reception
	clients[found].buffer=recv_buf;
	
	// Envoi - len=(n-1)*BL+1
	send_buf.resize((nb_clients()-1)*BUFFER_LEN+1);
	send_buf[0]=flag;
	int i=1;
	for(int c=0;c<nb_clients();c++)
	  {
	    if (c==found) continue;
	    for(int k=0;k<len_data();k++)
	      {
		send_buf[i]=clients[c].buffer[k];
		i++;
	      }
	  }
      }
    // Départ d'un client
    else if (found>=0 && flag==3)
      {
	// Reception
	isFree[found]=true;
	std::cout<<"Client quit"<<std::endl;

	// Envoi - len=4
	send_buf.resize(4);
	send_buf[0]=flag;
      }
    // Autre
    else
      {
	std::cerr<<"Strange data receive... [not implemented]"<<std::endl;
	return -1;
      }
    return flag;
  }
};


// Run the server
int main(int argc, char** argv) 
{
  int n=3,l=12,h=16;

  // Arguments

  // Global variables
  Server SERVEUR(n,l,h);
  bool keep_running=true;

  try
    {
      boost::asio::io_service io_service;
      udp::socket socket(io_service, udp::endpoint(udp::v4(), PORT));
  
      std::vector<uint32_t> recv_buf(BUFFER_LEN);
      std::vector<uint32_t> send_buf(4);
      udp::endpoint remote_endpoint;

      while(keep_running)
	{
	  boost::system::error_code error;
	  
	  socket.receive_from(boost::asio::buffer(recv_buf),
			      remote_endpoint, 0, error);

	  if (error && error != boost::asio::error::message_size)
	    throw boost::system::system_error(error);

	  SERVEUR.swap_data(recv_buf,send_buf,remote_endpoint);


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
