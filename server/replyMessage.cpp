#include "replyMessage.hpp"
#include "debug/debug.hpp"
#include "numreplies.hpp"

std::string sendTopicMessage(std::string host, std::size_t replyCode, std::string nickName, std::string channelName, std::string topicMessage)
{
	std::stringstream sendTopicMessage;
	sendTopicMessage << ":" << host << " " << replyCode << " " << nickName << " " << channelName << " :" << topicMessage << std::endl;
	topicMessage.clear();
	return (sendTopicMessage.str());
}

void sendBadChannelKeyError(int sd, std::string host, std::string nickName, std::string channelName)
{
	LOG("Sending ERR_BADCHANNELKEY error! errorcode: " << ERR_BADCHANNELKEY << " to: " << nickName);
	//NOTE	:host 475 * :Cannot join channel (+k)
	std::stringstream badChannelKeyError;
	badChannelKeyError << ":" << host << " " << ERR_BADCHANNELKEY << " * " << channelName << " :Cannot join channel (+k)" << std::endl;
	send(sd, badChannelKeyError.str().c_str(), badChannelKeyError.str().size(), 0);
}

void sendNoSuchChannelError(int sd, std::string host, std::string nickName, std::string channelName)
{
    LOG("Sending ERR_NOSUCHCHANNEL error! errorcode: " << ERR_NOSUCHCHANNEL << " to: " << nickName);
    //NOTE	"<channel name> :No such channel"
    std::stringstream noSuchChannelError;
    noSuchChannelError << ":" << host << " " << ERR_NOSUCHCHANNEL << " * " << channelName << " :No such channel" << std::endl;
    send(sd, noSuchChannelError.str().c_str(), noSuchChannelError.str().size(), 0);
}

void sendUserOnChannelError(int sd, std::string host, std::string nickName, std::string channelName)
{
    LOG("Sending ERR_USERONCHANNEL error! errorcode: " << ERR_USERONCHANNEL << " to: " << nickName);
    //NOTE  "<client> <nick> <channel> :is already on channel"
    std::stringstream noToChannelError;
    noToChannelError << ":" << host << " " << ERR_USERONCHANNEL << " * " << channelName << " :You're already on that channel" << std::endl;
    send(sd, noToChannelError.str().c_str(), noToChannelError.str().size(), 0);
}

std::string sendNameReply(std::string nickName, bool isOperator, std::size_t channelSize)
{
    LOG("Sending RPL_NAMEREPLY message! msgcode: " << RPL_NAMEREPLY << " to: " << nickName);
    //NOTE  :bar.example.com 353 amy = #test :@raoul jordi henk
	std::stringstream nameReply;
    nameReply << ((channelSize == 1) ? "" : " ") << ((isOperator == true) ? "@" : "") << nickName;
    return (nameReply.str());
}

void sendEndOfNamelist(int sd, std::string host, std::string nickName, std::string channelName)
{
	LOG("Sending RPL_ENDOFNAMES error! msgcode: " << RPL_ENDOFNAMES << " to: " << nickName);
	//NOTE	:host 366 rverscho channel :End of /NAMES list
	std::stringstream sendEndOfNamelist;
	sendEndOfNamelist << ":" << host << " " << RPL_ENDOFNAMES << " " << nickName << " " << channelName << " :End of /NAMES list" << std::endl;
	send(sd, sendEndOfNamelist.str().c_str(), sendEndOfNamelist.str().size(), 0);
}

void sendKickMessage(int sd, std::string host, std::string targetNickName, std::string channelName, std::string kickMessage)
{
    LOG("Sending KICK msg! " << "to host: " << host << " target: " << targetNickName << " and channel: " << channelName);
    //NOTE KICK #Finnish John :Speaking English
    //NOTE :axe1!~qwebirc@86-89-31-100.fixed.kpn.net KICK #test1 axenth1 :axe1
    std::stringstream sendKickMessage;
    sendKickMessage << ":" << host << " KICK " << channelName << " " << targetNickName << " :" << kickMessage << std::endl;
    send(sd, sendKickMessage.str().c_str(), sendKickMessage.str().size(), 0);
}

void sendChanOPrivsNeededError(int sd, std::string host, std::string channelName)
{
	//NOTE :testnet.inspircd.org 482 Ascrod #ascrod :You're not a channel operator
	LOG("Sending ERR_CHANOPRIVSNEEDED error! errorcode: " << ERR_CHANOPRIVSNEEDED << " to: " << host)
	std::stringstream sendChanOPrivsNeededError;
	sendChanOPrivsNeededError << ":" << host << " " << ERR_CHANOPRIVSNEEDED << " " << host << " " << channelName << " :You're not a channel operator" << std::endl;
	send(sd, sendChanOPrivsNeededError.str().c_str(), sendChanOPrivsNeededError.str().size(), 0);
}

void sendUserNotInChannelError(int sd, std::string host, std::string targetNickName, std::string channelName)
{
	LOG("Sending ERR_USERNOTINCHANNEL error! errorcode: " << ERR_USERNOTINCHANNEL << " to: " << host);
	//NOTE "<nick> <channel> :They aren't on that channel"
	std::stringstream sendUserNotInChannelError;
	sendUserNotInChannelError << ":" << host << " " << ERR_USERNOTINCHANNEL << " " << targetNickName << " " << channelName << " :They aren't on that channel" << std::endl;
	send(sd, sendUserNotInChannelError.str().c_str(), sendUserNotInChannelError.str().size(), 0);
}

void sendNotOnChannelError(int sd, std::string host, std::string channelName)
{
	//NOTE "<channel> :You're not on that channel"
	LOG("Sending ERR_NOTONCHANNEL error! errorcode: " << ERR_NOTONCHANNEL << " to: " << host);
	std::stringstream sendNotOnChannelError;
	sendNotOnChannelError << ":" << host << " " << ERR_NOTONCHANNEL << " " << channelName << " :You're not on that channel" << std::endl;
	send(sd, sendNotOnChannelError.str().c_str(), sendNotOnChannelError.str().size(), 0);
}

void sendNoNickNameGiven(int sd, std::string nickName)
{
	//NOTE ":No nickname given"
	LOG("Sending ERR_NONICKNAMEGIVEN error! errorcode: " << ERR_NONICKNAMEGIVEN << " to: " << nickName);
	std::stringstream sendNoNickNameGiven;
	sendNoNickNameGiven << ":" << nickName << ERR_NONICKNAMEGIVEN << " :No nickname given" << std::endl;
	send(sd, sendNoNickNameGiven.str().c_str(), sendNoNickNameGiven.str().size(), 0);
}

void sendErroneusNickName(int sd, std::string nickName)
{
	//NOTE "<nick> :Erroneus nickname"
	LOG("Sending ERR_ERRONEUSNICKNAME error! errorcode: " << ERR_ERRONEUSNICKNAME << " to: " << nickName);
	std::stringstream sendErroneusNickName;
	sendErroneusNickName << ":" << nickName << ERR_ERRONEUSNICKNAME << " :Erroneus nickname" << std::endl;
	send(sd, sendErroneusNickName.str().c_str(), sendErroneusNickName.str().size(), 0);
}

void sendNickNameInUseError(int sd, std::string nickName)
{
	//NOTE "<nick> :Nickname is already in use"
	LOG("Sending ERR_NICKNAMEINUSE error! errorcode: " << ERR_NICKNAMEINUSE << " to: " << nickName);
	std::stringstream sendNickNameInUseError;
	sendNickNameInUseError << ":" << nickName << ERR_NICKNAMEINUSE << " :Nick is already in use" << std::endl;//NOTE I see weird : in the message but I cant remove it withouth breaking the rplymsg
	send(sd, sendNickNameInUseError.str().c_str(), sendNickNameInUseError.str().size(), 0);
}

void sendNickColissionError(int sd, std::string nickName, const std::string hostName)
{
	//NOTE "<nick> :Nickname collision KILL"
	LOG("Sending ERR_NICKCOLISSION error! errorcode: " << ERR_NICKCOLISSION << " to: " << nickName);
	std::stringstream sendNickColission;
	sendNickColission << ":" << hostName << " " << ERR_NICKCOLISSION << " * " << nickName << " :Nickname collision KILL" << std::endl;
	send(sd, sendNickColission.str().c_str(), sendNickColission.str().size(), 0);
}

void partingMessage(std::string msg, std::string host, int sd, std::string nickName, std::string channelName)
{
    std::stringstream newmsg;
    if (nickName.empty() == false) {
        WARNING("User not found!");
    }
    if (msg.empty() == false) {
        newmsg << ":" << nickName << "!" << nickName << "@" << host << " PART " << channelName << " :" << msg
               << std::endl;
    }
    else if (msg.empty() == true) {
        newmsg << ":" << nickName << "!" << nickName << "@" << host << " PART " << channelName << std::endl;
    }
    send(sd, newmsg.str().c_str(), newmsg.str().size(), 0);
}
