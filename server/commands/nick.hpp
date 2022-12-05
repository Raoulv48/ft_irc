#ifndef NICK_HPP
#define NICK_HPP
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
#include "../inputparser/inputparser.hpp"
#include <map>
#include <sstream>

void sendNick(users::UserRegistration &users, std::string msg, int sd, std::map<std::string, Channel*> *channels, std::string buffer);

#endif
