#ifndef REPLYMESSAGE_HPP
#define REPLYMESSAGE_HPP
#include <iostream>
#include <sstream>
#include <sys/socket.h>

std::string sendTopicMessage(std::string host, std::size_t replyCode, std::string nickName, std::string channelName, std::string topicMessage);
void sendBadChannelKeyError(int sd, std::string host, std::string nickName, std::string channelName);
void sendNoSuchChannelError(int sd, std::string host, std::string nickName, std::string channelName);
void sendUserOnChannelError(int sd, std::string host, std::string nickName, std::string channelName);
std::string sendNameReply(std::string nickName, bool isOperator, std::size_t channelSize);
void sendEndOfNamelist(int sd, std::string host, std::string nickName, std::string channelName);
void sendKickMessage(int sd, std::string host, std::string targetNickName, std::string channelName, std::string kickMessage);
void sendChanOPrivsNeededError(int sd, std::string host, std::string channelName);
void sendUserNotInChannelError(int sd, std::string host, std::string targetNickName, std::string channelName);
void sendNotOnChannelError(int sd, std::string host, std::string channelName);

void sendNoNickNameGiven(int sd, std::string nickName);
void sendErroneusNickName(int sd, std::string nickName);

void sendNickNameInUseError(int sd, std::string nickName);
void sendNickColissionError(int sd, std::string nickName, const std::string hostName);

void partingMessage(std::string msg, std::string host, int sd, std::string nickName, std::string channelName);

#endif
