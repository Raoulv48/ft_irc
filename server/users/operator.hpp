#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "user.hpp"
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
#include "UserRegistration.hpp"
#include "../users/channel.hpp"
#include "../replyMessage.hpp"
#include <sstream>
#include <map>

void makeOperator(int sd, Channel* &channel, std::string targetNickName, std::string channelName, std::string nickName);
void removeOperator(int sd, Channel* &channel, std::string targetNickName, std::string channelName, std::string nickName);
void addChannelKey(std::string msg, std::map<std::string, Channel*> &channels);

#endif
