#include "part.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"

std::size_t partChannel(users::UserRegistration &users, int sd, std::string channelName, std::string msg, std::map<std::string, Channel*> &channels, InputParser &input)
{
    std::map <std::string, Channel*>::iterator it;
	Channel* tempChannel = channels.find(channelName)->second;
	it = channels.find(channelName);

	if (it != channels.end() && tempChannel->isInChannel(users.getUser(sd)->getNick()))
	{
        if (!tempChannel->isInChannel(users.getUser(sd)->getNick())) {
            WARNING("User " << users.getUser(sd)->getNick() << " is not in channel: " << channelName << " !");
            return (0);
        }

        for (int fd = 5; fd < MAX_CLIENTS + 5; fd++){
            users::user *temp = users.getUser(fd);
            if (temp != nullptr){
                if (tempChannel->isInChannel(users.getUser(fd)->getNick())) {
                    partingMessage(msg, users.getUser(sd)->getHost(), fd, users.getUser(sd)->getNick(), channelName);
                }
            }
        }
        //NOTE check if a new operator is needed for the channel
		if (tempChannel->_nickNameOperator.size() > 1) { //NOTE check if there is someone left to make him/her the operator
            int opCounter = 0;
            for (int fd = 5; fd < MAX_CLIENTS + 5; fd++) { //NOTE check of there is a operator left
                users::user *temp = users.getUser(fd);
                if (temp != nullptr && fd != sd) {
                    if (tempChannel->getOperator(temp->getNick())) {
                        opCounter++;
                        break;
                    }
                }
            }
            if (opCounter == 0) {
                for (int fd = 5; fd < MAX_CLIENTS + 5; fd++) { //NOTE create new operator of there is none left
                    users::user *temp = users.getUser(fd);
                    if (temp != nullptr && fd != sd) {
                        if (tempChannel->isInChannel(temp->getNick())) {
                            opCounter = fd;
                            break;
                        }
                    }
                }
                if (tempChannel->getOperator(users.getUser(sd)->getNick())) {
                    makeOperator(opCounter, tempChannel, users.getUser(opCounter)->getNick(), channelName, users.getUser(opCounter)->getNick());
                    std::stringstream opMsg;
                    opMsg << "MODE " << tempChannel->getChannel() << " +o" << " " << users.getUser(opCounter)->getNick() << std::endl;
                    for (int tempFd = 5; tempFd < MAX_CLIENTS + 5; tempFd++){
                        users::user *temp = users.getUser(tempFd);
                        if (temp != nullptr && tempFd != sd) {
                            if (tempChannel->isInChannel(temp->getNick())) {
                                send(users.getUser(tempFd)->getFd(), opMsg.str().c_str(), opMsg.str().size(), 0);
                            }
                        }
                    }
                }
            }
            tempChannel->_nickNameOperator.erase(users.getUser(sd)->getNick());
        }
        else if (tempChannel->_nickNameOperator.size() == 1) {
            std::string tempChannelName = tempChannel->getChannel();
            delete(tempChannel);
            channels.erase(tempChannelName);
        }
        LOG("user " << users.getUser(sd)->getNick() << " has left channel: " << channelName << " with message: " << msg);
    }
	else if (it == channels.end()) //NOTE no channel to part
    {
        sendNoSuchChannelError(users.getUser(sd)->getFd(), input.getHost(), users.getUser(sd)->getNick(), channelName);
    }
    else if (!tempChannel->isInChannel(users.getUser(sd)->getNick())) //NOTE user not in the channel
    {
    	sendNotOnChannelError(sd, users.getUser(sd)->getNick(), channelName);
    	}
    return (0);
}

std::size_t partEntry(users::UserRegistration &users, int sd, std::string &msg, std::string buffer, std::map<std::string, Channel*> &channels, InputParser &input)
{
	if (!partVerification(msg))
	{
		sendFailMsg(sd, users.getUser(sd)->getNick());
		msg.clear();
		buffer[0] = '\0';
		ERROR("Part failed");
		return (1);
	}

    std::map<std::size_t , std::pair<std::string, std::string> > channelNameMap = convertPartChannelNameMessage(msg);

    for(std::map<std::size_t , std::pair<std::string, std::string> >::const_iterator it = channelNameMap.begin(); it != channelNameMap.end(); ++it)
    {
        partChannel(users, sd, it->second.first, it->second.second, channels, input);
    }
    msg.clear();
    buffer[0] = '\0';
    return (0);
}
