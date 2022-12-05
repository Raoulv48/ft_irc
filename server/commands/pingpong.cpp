#include "pingpong.hpp"
#include "../debug/debug.hpp"
#include "../utils/commandVerification.hpp"

std::size_t pingPong(int sd, std::string &msg, std::string buffer)
{
	if (!pingVerification(msg))
	{
		sendFailMsg(sd, " ");
		msg.clear();
		buffer[0] = '\0';
		ERROR("Ping/Pong failed");
		return (1);
	}

	if (msg.find("PING") != std::string::npos) {
		std::stringstream replyMsg;
		replyMsg << "PONG " << msg.substr(5, msg.size() - 5) << std::endl;
		send(sd, replyMsg.str().c_str(), replyMsg.str().size(), 0);
	} else if (msg.find("PONG") != std::string::npos) {
		std::stringstream replyMsg;
		replyMsg << "PING " << msg.substr(5, msg.size() - 5) << std::endl;
		send(sd, replyMsg.str().c_str(), replyMsg.str().size(), 0);
	}
	msg.clear();
	buffer[0] = '\0';
	return (0);
}
