#pragma once

#include <cassert>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <unistd.h> // usleep

// Threads 
#include <boost/thread.hpp>

// Net
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
const int PORT = 1313;


void msleep(int ms){usleep(ms*1000);}
