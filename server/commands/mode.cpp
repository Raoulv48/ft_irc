#include "mode.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"

void modeEntry(users::UserRegistration &users, std::string &msg, std::map<std::string, Channel*> &channels, std::string buffer, int sd, InputParser &input)
{
	if (!modeVerification(msg))
	{
		sendFailMsg(sd, users.getUser(sd)->getNick());
		msg.clear();
		buffer[0] = '\0';
		ERROR("Mode failed");
		return;
	}

	if ((msg.find("+") != std::string::npos) || (msg.find("-")!= std::string::npos))
	{
		std::size_t pos1 = msg.find(" ") + 1;
		std::size_t pos2 = pos1;
		for (; pos2 < msg.size(); pos2++) {
			if (msg[pos2] == ' ' || msg[pos2] == '\n') {
                break;
            }
		}
		std::string channelName = msg.substr(pos1, pos2 - pos1);
		Channel *channel = channels.find(channelName)->second;
		std::map<std::string, Channel *>::iterator it = channels.find(channelName);
		if (it == channels.end()) {
			sendNoSuchChannelError(sd, users.getUser(sd)->getNick(), users.getUser(sd)->getNick(), channelName);
			return;
		}
		std::stringstream nameReply;//NOTE edit the names on client site
		nameReply << ":" << input.getHost() << " " << RPL_NAMEREPLY << " " << users.getUser(sd)->getNick() << " = "
				  << channelName << " :";
		std::string targetNickName;
		if (msg.find("+") || msg.find("-")) {
            pos2 += 4;
        }
		targetNickName = msg.substr(pos2, msg.size() - pos2 - 1);

		if (channel->getOperator(users.getUser(sd)->getNick())) {
            if (msg.find("+o") != std::string::npos) {
                makeOperator(sd, channel, targetNickName, channelName, users.getUser(sd)->getNick());
            }
			if (msg.find("-o") != std::string::npos) {
                removeOperator(sd, channel, targetNickName, channelName, users.getUser(sd)->getNick());
            }
            if (msg.find("+k") != std::string::npos) {
                addChannelKey(msg, channels);
            }
            //NOTE here we do things for operator

			if (((msg.find("+o") != std::string::npos) || (msg.find("-o") != std::string::npos)) &&
				channel->getOperator(users.getUser(sd)->getNick()) == true &&
				channel->isInChannel(targetNickName) == true) {
				for (int fd = 5; fd < MAX_CLIENTS + 5; fd++)
				{
					users::user *temp = users.getUser(fd);
					Channel *tempChannel = channels.find(channelName)->second;
					if (temp != nullptr) {
                        if (tempChannel->isInChannel(users.getUser(fd)->getNick()))
                        {
                            nameReply << sendNameReply(users.getUser(fd)->getNick(),
                                                       tempChannel->getOperator(users.getUser(fd)->getNick()),
                                                       tempChannel->_nickNameOperator.size());//NOTE add nick and operator to list
                        }
					}
				}
				nameReply << std::endl;

				for (int fd = 5; fd < MAX_CLIENTS + 5; fd++)
				{
					users::user *temp = users.getUser(fd);
					Channel *tempChannel = channels.find(channelName)->second;
					if (temp != nullptr) {
						send(temp->getFd(), nameReply.str().c_str(), nameReply.str().size(),
							 0); //NOTE send name list to all users
						std::stringstream test;
						bool opSign = tempChannel->getOperator(targetNickName);
						test << "MODE " << channelName << ((opSign == true) ? " +o" : " -o") << " " << targetNickName
							 << std::endl;
						send(temp->getFd(), test.str().c_str(), test.str().size(), 0);

					}
				}

			}
		} else if (!channel->getOperator(
				users.getUser(sd)->getNick())) {
            sendChanOPrivsNeededError(sd, users.getUser(sd)->getNick(), channelName);
        }
	}
	msg.clear();
	buffer[0] = '\0';
}
