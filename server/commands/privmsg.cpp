#include "privmsg.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"

std::size_t sendPrivmsg(int fd, std::string msg, users::user* user, users::user* user2)
{
	std::size_t pos1, pos2;
	pos1 = msg.find(" ");
	pos2 = msg.find(":");

	std::string usr = msg.substr(pos1, pos2 - pos1  -1);
	std::string msgToSend = msg.substr(pos2 + 1, msg.size() - pos2 + 2);

	std::stringstream newmsg;
	if (user == NULL) {
        ERROR("User is NULL");
    }
	newmsg << ":" << user2->getNick() <<"!"<<user2->getNick()<<"@"<<user2->getHost() << " PRIVMSG" << usr << " :" << msgToSend;

	return (send(fd, newmsg.str().c_str(), newmsg.str().size(), 0));
}

std::size_t sendChannelmsg(int sd, users::UserRegistration &users, users::user* user, std::string msg, std::map<std::string, Channel*> &channels)
{
	std::size_t pos1, pos2;
	pos1 = msg.find(" ");
	pos2 = msg.find(":");

	std::string channelName = msg.substr(pos1 + 1, pos2 - pos1  -2);
	std::string msgToSend = msg.substr(pos2 + 1, msg.size() - pos2 + 2);
	std::map<std::string, Channel*>::iterator it;
	it = channels.find(channelName);
	Channel* tempChannel = it->second;
	std::string test = it->first;

	std::stringstream newmsg;
	if (user == NULL) {
        ERROR("User is NULL");
    }
	newmsg << ":" << user->getNick() <<"!"<<user->getNick()<<"@"<<user->getHost() << " PRIVMSG " << channelName << " :" << msgToSend;

	for (int fd = 5; fd < MAX_CLIENTS + 5; fd++)
	{
		users::user *temp = users.getUser(fd);
		if (temp != nullptr) {
			if (tempChannel->isInChannel(users.getUser(fd)->getNick()) && fd != sd)
			{
				send(temp->getFd(), newmsg.str().c_str(), newmsg.str().size(), 0);
			}
		}
	}
	return (0);
}

void privmsgEntry(int sd, std::string &msg, std::string buffer, InputParser &input, users::UserRegistration &users, std::map<std::string, Channel*> &channels)
{
	if (!privmsgVerifictation(msg))
	{
		sendFailMsg(sd, users.getUser(sd)->getNick());
		msg.clear();
		buffer[0] = '\0';
		ERROR("Privmsg failed");
		return;
	}

	std::string tempusr = msg.substr(msg.find(" ") +1 , msg.size() - (msg.find((" ") + 1)));
	std::string tempuser;
	if (tempusr[0] == '#') {
        tempuser = tempusr.substr(0, msg.find(" ") - 2);
    }
	else {
		int i = 0;
		while (tempusr[i] != ' ')
			i++;
		tempuser = tempusr.substr(0, i);
	}
	users::user *temp2 = users.getUser(sd);
	if (tempuser[0] == '#') {
		sendChannelmsg(sd, users, temp2, msg, channels);
		return;
	}
	users::user *temp = users.getUser(tempuser);
	if (temp == nullptr && tempuser[0] != '#')
	{
		std::stringstream replymsg;
		replymsg << ":" << input.getHost() << " " << ERR_NOSUCHNICK << " " << users.getUser(sd)->getNick() << " " << tempusr << " :No such nick/channel" << std::endl;
		send(sd, replymsg.str().c_str(), replymsg.str().size(), 0);
		return;
	}
	int fd = temp->getFd();
	sendPrivmsg(fd, msg, users.getUser(sd), temp2);
	msg.clear();
	buffer[0] ='\0';
}
