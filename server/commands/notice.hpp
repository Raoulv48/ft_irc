#ifndef NOTICE_HPP
#define NOTICE_HPP

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

#include <sstream>

std::size_t sendNotice(int fd, std::string msg, users::user* user, users::user* user2);
std::size_t sendChannelNotice(int sd, users::UserRegistration &users, users::user* user, std::string msg, std::map<std::string, Channel*> &channels);
void noticeEntry(int sd, std::string &msg, std::string buffer, InputParser &input, users::UserRegistration &users, std::map<std::string, Channel*> &channels);

#endif
