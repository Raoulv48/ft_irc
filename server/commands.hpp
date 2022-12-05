#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sstream>
#include <fcntl.h>
#include "debug/debug.hpp"
#include "utils/commandUtils.hpp"
#include "users/topic.hpp"
#include <map>
#include "users/UserRegistration.hpp"
#include "commands/part.hpp"
#include "commands/join.hpp"
#include "commands/privmsg.hpp"
#include "commands/pingpong.hpp"
#include "commands/nick.hpp"
#include "commands/mode.hpp"
#include "commands/kick.hpp"
#include "commands/notice.hpp"
#include "users/channel.hpp"
#include "commands.hpp"
#include "server.hpp"
#include "utils/MessageVerification.hpp"
#include "utils/commandVerification.hpp"

void commands(users::UserRegistration &users,
              int sd, std::map<std::string,
              Channel*> *channels,
              std::size_t &channelCount,
              char *buffer,
              InputParser &input,
              server &ircServer,
              std::vector<users::t_authData> &authData,
              std::map<int, std::pair<std::string, bool> > &authMap);

#endif
