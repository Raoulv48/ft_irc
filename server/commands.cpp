#include "commands.hpp"
#include "debug/debug.hpp"
#include <cstring>


void commands(users::UserRegistration &users,
              int sd,
              std::map<std::string, Channel*> *channels,
              std::size_t &channelCount,
              char *buffer,
              InputParser &input,
              server &ircServer,
              std::vector<users::t_authData> &authData,
              std::map<int, std::pair<std::string, bool> > &authMap)
{

	std::string incBuffer = ircServer.getPartialMessage(sd);
	std::memset(buffer, '\0', 1024);
	
	std::string msg(incBuffer);
	std::vector <std::string> messages = splitMsg(msg);
	for (std::size_t i = 0;
         i < messages.size(); i++) {
		msg = messages[i];
        if (isCommand("KICK", msg)) {
			kickEntry(users, sd, channels, msg, incBuffer, "");
			return;
		}
		if (isCommand("MODE", msg)) {
			if (msg[5] == '#') {
				modeEntry(users, msg, *channels, incBuffer, sd, input);
			}
			return;
		}
		if (isCommand("PING", msg) || isCommand("PONG", msg)) {
			pingPong(sd, msg, incBuffer);
			return;
		}
		if (isCommand("NICK", msg)) {
			sendNick(users, msg, sd, channels, incBuffer);
			return;
		}
		if (isCommand("USER", msg))
			return;
		if (isCommand("PRIVMSG", msg)) {
			privmsgEntry(sd, msg, incBuffer, input, users, *channels);
			return;
		}
		if (isCommand("JOIN", msg)) {
			joinEntry(users, sd, channels, channelCount, msg, incBuffer, input);
			return;
		}
		if (isCommand("TOPIC", msg)) {
			topicEntry(users, msg, sd, input.getHost(), incBuffer, *channels);
			return;
		}
		if (isCommand("PART", msg)) {
			partEntry(users, sd, msg, incBuffer, *channels, input);
			return;
		}
		if (isCommand("QUIT", msg)) {
			msg.clear();
			incBuffer[0] = '\0';
			ircServer.disconnect(users, sd, authData, authMap, users.getUser(sd)->getNick(), *channels);
			return;
		}
		if (isCommand("NOTICE", msg)) {
		    noticeEntry(sd, msg, incBuffer, input, users, *channels);
		    return;
        }
		if (isCommand("WHOIS", msg) || isCommand("WHO", msg)){
			msg.clear();
			incBuffer[0] = '\0';
			return;
		}
		sendFailMsg(sd, users.getUser(sd)->getNick());
		incBuffer[0] = '\0';
	}
}
