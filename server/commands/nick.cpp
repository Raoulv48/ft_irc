#include "nick.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"

bool nickbullshitreturner(std::string nickToCheck)
{
	for (std::size_t i = 0; nickToCheck[i] != '\0'; i++)
	{
		if (nickToCheck[i] == '#' || nickToCheck[i] == '&') {
            return (false);
        }
	}
	return (true);
}

void sendNick(users::UserRegistration &users, std::string msg, int sd, std::map<std::string, Channel*> *channels, std::string buffer)
{
	if (!nickVerification(msg))
	{
		sendFailMsg(sd, users.getUser(sd)->getNick());
		msg.clear();
		buffer[0] = '\0';
		ERROR("Nick failed");
		return;
	}

	users::user* temp = users.getUser(sd);
	std::size_t pos1 = msg.find(" ") + 1;
	std::string newNick = msg.substr(pos1, msg.size() - pos1 - 1);
	bool exists = false;

	if (newNick.size() == 0) //NOTE check if newNick is empty
	{
		sendNoNickNameGiven(sd, users.getUser(sd)->getNick());
        msg.clear();
        buffer[0] = '\0';
		return;
	}

	if (!nickbullshitreturner(newNick))
	{
		sendErroneusNickName(sd, users.getUser(sd)->getNick());
        msg.clear();
        buffer[0] = '\0';
		return;
	}

	for (int fd = 5; fd < MAX_CLIENTS + 5; fd++)
	{
		users::user *temp = users.getUser(fd);
		if (temp != nullptr)
		{
			if (!temp->getNick().compare(newNick))
			{
				sendNickNameInUseError(sd, newNick);
				exists = true;
			}
		}
	}
	LOG("trying to change " << temp->getNick() << " to [" << newNick << "]");
	if (exists == false)
	{
		std::stringstream nickMsg;
		nickMsg << ":" << temp->getNick() << " NICK " << newNick.c_str() << std::endl;
		send(sd, nickMsg.str().c_str(), nickMsg.str().size(), 0);//NOTE sends newNick to client otherwise you'll have a missmatch
		std::size_t i;
		for (i = 0; i < temp->_channelName.size(); i++) {
			Channel *tempChannel = channels->find(temp->_channelName[i])->second;
			if (tempChannel->isInChannel(temp->getNick()) == true) {
				std::string tempOpSet;
				(tempChannel->getOperator(temp->getNick()) == true) ? tempOpSet = "+o" : tempOpSet = "-o"; //NOTE set OP rights
				tempChannel->setOperator(newNick.c_str(), tempOpSet); //NOTE create a new Nick with his old rights
				tempChannel->_nickNameOperator.erase(tempChannel->_nickNameOperator.find(temp->getNick())); //NOTE delete the old nick
                for (int fd = 5; fd < MAX_CLIENTS + 5; fd++){
                    users::user *temp2 = users.getUser(fd);
                    if (temp2 != nullptr && fd != sd){
                        send(temp2->getFd(), nickMsg.str().c_str(), nickMsg.str().size(), 0);
                    }

                }
			}
		}
		temp->setNick(newNick.c_str()); //NOTE setNewNick
	}
	msg.clear();
	buffer[0] = '\0';
}
