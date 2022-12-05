#ifndef KICK_HPP
#define KICK_HPP
#include <iostream>
#include "../users/UserRegistration.hpp"
#include "../replyMessage.hpp"
#include "../users/topic.hpp"
#include "../utils/commandUtils.hpp"
#include "../users/channel.hpp"
#include "../inputparser/inputparser.hpp"
#include "../users/operator.hpp"
#include <map>
#include <sstream>
#include <errno.h>
#include <string>

void kickUser(users::UserRegistration &users, int sd, std::string channelName, std::string targetNickName, std::string kickMessage, Channel* channel);
void kickEntry(users::UserRegistration &users, int sd, std::map<std::string, Channel*> *channels, std::string &msg, std::string buffer, std::string targetNickName);

#endif
