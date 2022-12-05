#include "../debug/debug.hpp"
#include "operator.hpp"

void makeOperator(int sd, Channel* &channel, std::string targetNickName, std::string channelName, std::string nickName)
{

	if (!channel->isInChannel(targetNickName))
	{
		sendUserNotInChannelError(sd, nickName, targetNickName, channelName);
		return;
	}
	channel->setOperator(targetNickName, "+o");
}

void removeOperator(int sd, Channel* &channel, std::string targetNickName, std::string channelName, std::string nickName)
{

	if (!channel->isInChannel(targetNickName))
	{
		sendUserNotInChannelError(sd, nickName, targetNickName, channelName);
		return;
	}
	channel->setOperator(targetNickName, "-o");
}


void addChannelKey(std::string msg, std::map<std::string, Channel*> &channels)
{
	std::size_t uPos1, cPos1, cPos2;
	std::string tempChannelKey, tempChannelName;

	cPos1 = msg.find(" ") + 1;
	cPos2 = msg.find("+k ") - 1;
	tempChannelName = msg.substr(cPos1, cPos2 - cPos1);
	uPos1 = msg.find("+k ") + 3;
	tempChannelKey = msg.substr(uPos1, msg.size() - uPos1 - 1);

	Channel* tempChannel = channels.find(tempChannelName)->second;
	LOG("channelKey: [" << tempChannelKey << "] for " << tempChannelName)
	tempChannel->setChannelKey(tempChannelKey);
}

