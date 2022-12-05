#include "kick.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"
#include <string.h>

void kickUser(users::UserRegistration &users, int sd,  std::string channelName, std::string targetNickName, std::string kickMessage, Channel* channel)
{
	for (int fd = 5; fd < MAX_CLIENTS + 5; fd++)
	{
		users::user *temp = users.getUser(fd);
		if (temp != nullptr && channel->isInChannel(temp->getNick()))
		{
				int i = 1;
				sendKickMessage(users.getUser(fd)->getFd(), users.getUser(sd)->getHost(), targetNickName, channelName,	kickMessage);
				i++;
		}
	}
	channel->_nickNameOperator.erase((channel->_nickNameOperator.find(users.getUser(targetNickName)->getNick())));//NOTE remove the user from the database
}

void kickEntry(users::UserRegistration &users, int sd, std::map<std::string, Channel*> *channels, std::string &msg, std::string buffer, std::string targetNickName)
{
	if (!kickVerification(msg))
	{
		sendFailMsg(sd, users.getUser(sd)->getNick());
		msg.clear();
		buffer[0] = '\0';
		ERROR("Kick failed");
		return;
	}

    std::size_t pos1 = msg.find("#");
    std::size_t pos2 = pos1;
    for (;pos2 < msg.size(); pos2++)//NOTE loop to get the channelName
    {
        if (msg[pos2] == ' ' || msg[pos2] == '\n') {
            break;
        }
    }
    std::string channelName = msg.substr(pos1, pos2 - pos1);
    Channel* channel = channels->find(channelName)->second;
    std::map <std::string, Channel*>::iterator it = channels->find(channelName);
    if (it == channels->end())
	{
		sendNoSuchChannelError(sd, users.getUser(sd)->getNick(), users.getUser(sd)->getNick(), channelName);
		return;
	}
    if (channel->isInChannel(users.getUser(sd)->getNick())) { //NOTE checks if user is in channel
		if (channel->getOperator(users.getUser(sd)->getNick())) { //NOTE checks if user is allowed to kick
			pos2 += 1;
			pos1 = pos2;
			for (; pos2 < msg.size(); pos2++) { //NOTE loop to get the target username
				if (msg[pos2] == ' ' || msg[pos2] == '\n') {
                    break;
                }
			}
			targetNickName = msg.substr(pos1, pos2 - pos1);

			if (channel->isInChannel(targetNickName)) { //NOTE check if the user is in the channel
				pos2 += 2; //NOTE this will set the position behind the : for the message, this will always be 2 places after the targetNickName
				pos1 = pos2;
				for (; pos2 < msg.size(); pos2++) {
					if (msg[pos2] == '\n' || msg[pos2] == '\0' || msg[pos2] == '\r') {
                        break;
                    }
				}
				std::string kickMessage = "";
				if (pos2 > pos1) {
                    kickMessage = msg.substr(pos1, pos2 - pos1);
                }
                //NOTE check if a new operator is needed for the channel
				int fd = 5;
				if (channel->_nickNameOperator.size() > 1) { //NOTE check if there is someone left to make him/her the operator
                    int opCounter = 0;
                    for (; fd < MAX_CLIENTS + 5; fd++) { //NOTE check of there is a operator left
                        users::user *temp = users.getUser(fd);
                        if (temp != nullptr && fd != sd) {
                            if (channel->getOperator(temp->getNick())) {
                                opCounter++;
                                break;
                            }
                        }
                    }
                    if (opCounter == 0) {
                        for (int fd = 5; fd < MAX_CLIENTS + 5; fd++) { //NOTE create new operator of there is none left
                            users::user *temp = users.getUser(fd);
                            if (temp != nullptr && fd != sd) {
                                if (channel->isInChannel(temp->getNick())) {
                                    opCounter = fd;
                                    break;
                                }
                            }
                        }
                        if (!targetNickName.compare(users.getUser(sd)->getNick())) {
                            makeOperator(opCounter, channel, users.getUser(opCounter)->getNick(), channelName,
                                         users.getUser(opCounter)->getNick());
                            std::stringstream opMsg;
                            opMsg << "MODE " << channelName << " +o" << " " << users.getUser(opCounter)->getNick()
                                  << std::endl;
                            send(users.getUser(opCounter)->getFd(), opMsg.str().c_str(), opMsg.str().size(), 0);
                        }
                    }
                    kickUser(users, sd, channelName, targetNickName, kickMessage, channel);
                }
                else if (channel->_nickNameOperator.size() == 1) {
                    kickUser(users, sd, channelName, targetNickName, kickMessage, channel);
                    std::string tempChannelName = channel->getChannel();
                    delete(channel);
                    channels->erase(tempChannelName);
                }
			}
			else if (!channel->isInChannel(targetNickName))//NOTE send if target isn't in the channel
            {
                sendUserNotInChannelError(sd, users.getUser(sd)->getNick(), targetNickName, channelName);
            }
		}
		else if(!channel->getOperator(users.getUser(sd)->getNick()))//NOTE send if user needs channel privileges
			sendChanOPrivsNeededError(sd, users.getUser(sd)->getNick(), channelName);
	}
	else if (!channel->isInChannel(users.getUser(sd)->getNick()))//NOTE send if the user isn't in the channel using a command
    {
        sendNotOnChannelError(sd, users.getUser(sd)->getNick(), channelName);
    }
	msg.clear();
	buffer[0] = '\0';
}
