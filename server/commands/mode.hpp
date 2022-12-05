#ifndef MODE_HPP
#define MODE_HPP
#include "../users/user.hpp"
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
#include <sstream>
#include <map>
#include "../users/operator.hpp"
#include "../users/channel.hpp"
#include "../inputparser/inputparser.hpp"
#include "../replyMessage.hpp"

void modeEntry(users::UserRegistration &users, std::string &msg, std::map<std::string, Channel*> &channels, std::string buffer, int sd, InputParser &input);

#endif
