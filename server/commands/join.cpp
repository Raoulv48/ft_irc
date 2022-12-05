#include "join.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"
#include <ios>

std::size_t joinChannel(users::UserRegistration &users, int sd, std::map<std::string, Channel*> *channels, std::size_t &channelCount, std::string channelName, std::string channelPassword, InputParser &input)
{
	std::map <std::string, Channel*>::iterator it;
	it = channels->find(channelName);
	if (it == channels->end())
	{
		Channel* tempChannel = new Channel(channelName, "", channelCount, users.getUser(sd)->getNick(), "+o");
		channels->insert(std::make_pair(channelName, tempChannel));
		channelCount++;
		users.getUser(sd)->addChannel(channelName);//NOTE to keep track of the channels in the users
	}
	else
	{
	    if((channels->find(channelName)->second->getChannelKey() == "x" && channelPassword == "x") || (channels->find(channelName)->second->getChannelKey() == "x" && channelPassword.empty() == true) || (channels->find(channelName)->second->getChannelKey().empty() && channelPassword == "x"))
        {
	        ;
        }
		else if (channels->find(channelName)->second->getChannelKey().compare(channelPassword))
        {
            sendBadChannelKeyError(users.getUser(sd)->getFd(), input.getHost(), users.getUser(sd)->getNick(), channelName);
			return(1);//NOTE, user isn't allowed to join because of a wrong password
        }
		else if (channels->find(channelName)->second->isInChannel(users.getUser(sd)->getNick()))
        {
		    sendUserOnChannelError(users.getUser(sd)->getFd(), input.getHost(), users.getUser(sd)->getNick(), channelName);
            return (1); //NOTE, users has already joined the channel
        }
        users.getUser(sd)->addChannel(channelName);//NOTE to keep track of the channels in the users
		channels->find(channelName)->second->setOperator(users.getUser(sd)->getNick(), "-o");
	}
	std::stringstream newmsg;
	if (users.getUser(sd) == nullptr)
	{
		CRITICAL("user doesn't exist while joining, terminating program!");
	}
	newmsg << ":" << users.getUser(sd)->getNick() << "!" << users.getUser(sd)->getNick() << "@" << users.getUser(sd)->getHost() << " JOIN " << channelName << std::endl; //NOTE preparation for RPL_NAMEREPLY
	std::stringstream nameReply;
	nameReply << ":" << input.getHost() << " " << RPL_NAMEREPLY << " " << users.getUser(sd)->getNick() << " = " << channelName << " :";

	for (int fd = 5; fd < MAX_CLIENTS + 5; fd++)
	{
		users::user *temp = users.getUser(fd);
		Channel* tempChannel = channels->find(channelName)->second;
		if (temp != nullptr) {
			if (tempChannel->isInChannel(users.getUser(fd)->getNick())) {
				LOG(newmsg.str());
				send(temp->getFd(), newmsg.str().c_str(), newmsg.str().size(), 0);
				nameReply << sendNameReply(users.getUser(fd)->getNick(), tempChannel->getOperator(users.getUser(fd)->getNick()), tempChannel->_nickNameOperator.size());//NOTE add nick and operator to list
			}
		}
	}
	Channel* tempChannel = channels->find(channelName)->second;
	if 	(tempChannel->getTopicMessage().empty() == false)
	{
		std::string topicMessage = sendTopicMessage(users.getUser(sd)->getHost(), 332, users.getUser(sd)->getNick(), channelName, tempChannel->getTopicMessage());
		send(users.getUser(sd)->getFd(), topicMessage.c_str(), topicMessage.size(), 0);
	}

	nameReply << std::endl;
	for (int fd = 5; fd < MAX_CLIENTS + 5; fd++)
	{
		users::user *temp = users.getUser(fd);
		if (temp != nullptr)
		{
			if (users.getUser(sd) == users.getUser(fd))
			{
				send(temp->getFd(), nameReply.str().c_str(), nameReply.str().size(), 0); //NOTE send name list to all users
				sendEndOfNamelist(temp->getFd(), input.getHost(), temp->getNick(), channelName);
			}
		}
	}
	return (0);
}

void joinEntry(users::UserRegistration &users, int sd, std::map<std::string, Channel*> *channels, std::size_t &channelCount, std::string &msg, std::string buffer, InputParser &input)
{
	if (!joinVerification(msg))
	{
		sendFailMsg(sd, users.getUser(sd)->getNick());
		msg.clear();
		buffer[0] = '\0';
		ERROR("Join failed");
		return;
	}

	std::map<std::size_t , std::pair<std::string, std::string> > channelNameMap = convertMsgToChannelName(msg);

	//NOTE splitting the command in a MAP for channel join and password check
	for(std::map<std::size_t , std::pair<std::string, std::string> >::const_iterator it = channelNameMap.begin(); it != channelNameMap.end(); ++it) {
        joinChannel(users, sd, channels, channelCount, it->second.first, it->second.second, input);
    }
	msg.clear();
	buffer[0] = '\0';
}
