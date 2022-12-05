#include "topic.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"

static void topicBuilder(users::UserRegistration &users, std::string msg, int sd, std::string host, std::map<std::string, Channel*> &channels)
{
	std::size_t pos1 = msg.find(" ") + 1;
	std::size_t pos2 = msg.find(":") - 1;
	std::string channelName = msg.substr(pos1, pos2 - pos1);

	Channel* tempChannel = channels.find(channelName)->second;
	std::map <std::string, Channel*>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		if (tempChannel->getOperator(users.getUser(sd)->getNick()))
		{
			pos1 = msg.find(" :") + 2;
			std::string topicMessage = msg.substr(pos1, msg.size() - pos1 - 1);
			tempChannel->setTopicMessage(topicMessage);
			topicMessage = sendTopicMessage(host, 332, users.getUser(sd)->getNick(), channelName, topicMessage);
			for (int fd = 5; fd < MAX_CLIENTS + 5; fd++) {
				users::user *temp = users.getUser(fd);
				if (temp != nullptr) {
					if (tempChannel->isInChannel(temp->getNick())) {
						send(temp->getFd(), topicMessage.c_str(), topicMessage.size(), 0);
						LOG("Topic text: [" << topicMessage << "] on channel " << channelName);
					}
				}
			}
		}
		else if(!tempChannel->getOperator(users.getUser(sd)->getNick())) {
            sendChanOPrivsNeededError(sd, users.getUser(sd)->getNick(), channelName);
        }
	}
	else if (it == channels.end()) {
        sendNoSuchChannelError(sd, users.getUser(sd)->getNick(), users.getUser(sd)->getNick(), channelName);
    }
}

void topicEntry(users::UserRegistration &users, std::string &msg, int sd, std::string host, std::string buffer, std::map<std::string, Channel*> &channels)
{
	if (!topicVerification(msg))
	{
		sendFailMsg(sd, users.getUser(sd)->getNick());
		msg.clear();
		buffer[0] = '\0';
		ERROR("Topic failed");
		return;
	}

	topicBuilder(users, msg, sd, host, channels);
	msg.clear();
	buffer[0] = '\0';
}
