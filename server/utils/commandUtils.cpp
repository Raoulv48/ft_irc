#include "commandUtils.hpp"
#include "../debug/debug.hpp"

bool isCommand(std::string cmd, std::string msg) {
    if (msg.empty() == true || cmd.empty() == true) {
        return (false);
    }
    std::size_t pos1 = msg.find(" ");
    std::string newCmd = msg.substr(0, pos1);
    if (newCmd.compare(cmd) == 0) {
        return (true);
    }
	return (false);
}

std::map<std::size_t , std::pair<std::string, std::string> > convertMsgToChannelName(std::string msg)
{

	std::map<std::size_t, std::pair<std::string, std::string> > channelNameAndKey;
	std::size_t i, j, k, l;

	i = msg.find("#");
	j = i;
	k = 0;
	l = 0;

	while (i < msg.size())
	{
		if (msg[i] == '#' || msg[i] == '&')
		{
			while (msg[i] != ',' && msg[i] != '\0' && msg[i] != ' ' && msg[i] != '\n') {
                i++;
            }
			std::stringstream newMsg;
			std::string msgToPush = msg.substr(j, i - j);
			newMsg << msgToPush;
			channelNameAndKey.insert(std::make_pair(l, std::make_pair(newMsg.str(), "")));
			j = i + 1;
			l++;
		}
		else if (msg[i] != '#' && msg[i] != '&')
		{
			while (msg[i] != ',' && msg[i] != '\0' && msg[i] != '\n') {
                i++;
            }
			std::stringstream newMsg;
			std::string msgToPush = msg.substr(j, i - j);
			newMsg << msgToPush;
			channelNameAndKey.find(k)->second.second = newMsg.str();
			j = i + 1;
			k++;
		}
		i++;
	}
	return (channelNameAndKey);
}

std::map<std::size_t , std::pair<std::string, std::string> > convertPartChannelNameMessage(std::string msg)
{
    std::map<std::size_t, std::pair<std::string, std::string> > channelNameAndMsg;
    std::size_t i, j, l, m;

    i = msg.find(" ") + 1;
    j = i;
    l = 0;
    if ((m = msg.find(":")) == std::string::npos)
        m = msg.size();

    while (i < m && msg[i] != '\0' && msg[i] != '\n')
    {
        if (msg[i] == ',' || msg[i] == ' ' || msg[i + 1] == '\0' || msg[i + 1] == '\n')
        {
            if (msg[i + 1] == '\0' || msg[i + 1] == '\n') {
                i++;
            }
            std::stringstream newMsg;
            std::string msgToPush = msg.substr(j, i - j);
            newMsg << msgToPush;
            channelNameAndMsg.insert(std::make_pair(l, std::make_pair(newMsg.str(), "")));
            j = i + 1;
            l++;
        }
        i++;
    }
    if (msg[m] == ':')
    {
        while (msg[j] == ':' || msg[j] == ' ')
            j++;
        while (msg[i] != '\0' && msg[i] != '\n')
            i++;
        std::stringstream newMsg;
        std::string msgToPush = msg.substr(j, i - j);
        newMsg << msgToPush;
        for (std::size_t k = 0; k < l; k++) {
            channelNameAndMsg.find(k)->second.second = newMsg.str();
        }
    }
    return (channelNameAndMsg);
}


std::vector<std::string> splitMsg(std::string msg)
{
	std::vector<std::string> messages;

	std::size_t i, j;
	i = 0;
	j = 0;
	while (i < msg.size() && msg[i] != '\n')
	{
		if (msg[i] == '\0')
		{
			break;
		}
		if (msg[i] == '\r')
		{
			std::stringstream newMsg;
			std::string msgToPush = msg.substr(j, i - j);
			newMsg << msgToPush << std::endl;
			messages.push_back(newMsg.str());
			i += 2;
			j = i;
		}
		i++;
	}
	return (messages);
}
