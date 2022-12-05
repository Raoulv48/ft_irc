#ifndef PINGPONG_HPP
#define PINGPONG_HPP
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "../users/UserRegistration.hpp"
#include "../replyMessage.hpp"
#include "../users/topic.hpp"
#include <map>
#include <sstream>

std::size_t pingPong(int sd, std::string &msg, std::string buffer);

#endif
