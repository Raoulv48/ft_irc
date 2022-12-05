#ifndef PART_HPP
#define PART_HPP
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
#include "../users/channel.hpp"
#include "../utils/commandUtils.hpp"
#include "../inputparser/inputparser.hpp"
#include "../replyMessage.hpp"
#include "../users/operator.hpp"
#include <sstream>


std::size_t partChannel(users::UserRegistration &users, int sd, std::string channel, std::string msg, std::map<std::string, Channel*> &channels, InputParser &input);
std::size_t partEntry(users::UserRegistration &users, int sd, std::string &msg, std::string buffer, std::map<std::string, Channel*> &channels, InputParser &input);

#endif
