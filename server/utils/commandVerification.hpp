#ifndef COMMANDVERIFICATION_HPP
#define COMMANDVERIFICATION_HPP

#include <iostream>

void sendFailMsg(int sd, std::string nickname);
bool authVerification(std::string msg);
bool joinVerification(std::string msg);
bool kickVerification(std::string msg);
bool modeVerification(std::string msg);
bool pingVerification(std::string msg);
bool nickVerification(std::string msg);
bool partVerification(std::string msg);
bool privmsgVerifictation(std::string msg);
bool topicVerification(std::string msg);
bool noticeVerification(std::string msg);

#endif
