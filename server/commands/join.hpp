#ifndef JOIN_HPP
#define JOIN_HPP
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
#include "../utils/commandUtils.hpp"
#include "../users/channel.hpp"
#include "../inputparser/inputparser.hpp"
#include <map>
#include <sstream>

std::size_t joinChannel(users::UserRegistration &users, int sd, std::string channelName, std::map<std::string, Channel*> *channels, std::size_t &channelCount, InputParser &input);
void joinEntry(users::UserRegistration &users, int sd, std::map<std::string, Channel*> *channels, std::size_t &channelCount, std::string &msg, std::string buffer, InputParser &input);

#endif
