#ifndef TOPIC_HPP
#define TOPIC_HPP
#include "user.hpp"
#include "UserRegistration.hpp"
#include "../replyMessage.hpp"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include "../users/channel.hpp"
#include <map>

void topicEntry(users::UserRegistration &users, std::string &msg, int sd, std::string host, std::string buffer, std::map<std::string, Channel*> &channels);

#endif
