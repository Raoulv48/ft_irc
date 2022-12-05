#ifndef COMMAND_UTILS_HPP
# define COMMAND_UTILS_HPP
# include <iostream>
# include <vector>
# include <sstream>
# include <map>
# include "../users/channel.hpp"

bool isCommand(std::string cmd, std::string msg);
std::map<std::size_t , std::pair<std::string, std::string> > convertMsgToChannelName(std::string msg);
std::map<std::size_t , std::pair<std::string, std::string> > convertPartChannelNameMessage(std::string msg);
std::vector<std::string> splitMsg(std::string msg);

#endif
