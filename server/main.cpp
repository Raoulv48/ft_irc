#include "inputparser/inputparser.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sstream>
#include "debug/debug.hpp"
#include "utils/commandUtils.hpp"
#include "users/topic.hpp"
#include <map>
#include "users/channel.hpp"
#include "commands.hpp"
#include "utils/MessageVerification.hpp"
#include "utils/commandVerification.hpp"
#include "server.hpp"

#ifndef MAX_CLIENTS
    #define MAX_CLIENTS 30
#endif

 int main(int argc, char**argv)
{
    if (argc != 3) {
        CRITICAL("Expected usage: ircserv <port> <password>");
    }
    else
    {
        for (std::size_t i = 0 ; i < strlen(argv[1]); i++)
        {
            if (!std::isdigit(argv[1][i])) {
                CRITICAL(argv[1] << " is not a valid port!");
            }
        }
    }

	#if ENABLE_LOGGING == true
		time_t now = time(0);
		tm *ltm = localtime(&now);
		std::stringstream fileName;
		fileName << "logs/" << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year << "-" << ltm->tm_hour << ":" << ltm->tm_min << ".log";
		outlog.open(fileName.str().c_str());
	#endif

	InputParser input(argv, argc);
	server ircServer(input.getPort(), input.getPassword(), input);

	users::UserRegistration users;
	std::map<std::string, Channel*> channels;
	std::size_t channelCount = 0;

	std::map<int, std::pair<std::string, bool> > authMap;
	std::vector<users::t_authData> authData;

	std::string msg = "";

	int i = 0, sd, valread;
	while(1)
	{

	    ircServer.Select();


			if (FD_ISSET(ircServer.getMasterSocket(), ircServer.getReadfds()))
			{
				if (!ircServer.Accept(users, authData, authMap, channels)) {
                    continue;
                }
                LOG("New connection, socket fd is " << ircServer.getNewSocket() << ", ip is : "
													<< inet_ntoa(ircServer.getAddress()->sin_addr) << ", port: "
													<< ntohs(ircServer.getAddress()->sin_port))
				authMap.insert(std::make_pair(ircServer.getNewSocket(), std::make_pair(std::string(""), false)));
				authData.push_back(
						users::t_authData(ircServer.getNewSocket(), inet_ntoa(ircServer.getAddress()->sin_addr),
										  ntohs(ircServer.getAddress()->sin_port)));
				LOG(ircServer.getNewSocket() << " is ready for authentication");
			}

		for(i = 0; i < MAX_CLIENTS; i++)
		{
			sd = ircServer.getClientSocket(i);

			//NOTE Authenticate if user is not yet authenticated
			if (authMap.find(sd)->second.second == false && sd > 4)
			{
				std::string newBuffer = authMap.find(sd)->second.first;
				if (authMsgComplete(newBuffer))
				{
					if (!authVerification(newBuffer))
					{
						ERROR("Failed authentication");
						ircServer.disconnect(users, sd, authData, authMap, "", channels);
						continue;
					}
					std::string test(newBuffer);
					std::size_t pos1, pos2;
					pos1 = test.find("NICK");
					pos2 = test.find("\n", pos1 + 1);
					std::string nick, user, nickname;
					nick = test.substr(pos1, pos2 - pos1 - 1);
					user = test.substr(pos2 + 1, test.size() - pos2 + 1);
					nickname = nick.substr(5, nick.size() - 5);
					std::string password;
					users::user *tempUser1;
					std::size_t passPos = newBuffer.find("PASS");

					if (passPos == std::string::npos)
					{
						ERROR("Unable to find PASS message, terminating connection");
						ircServer.disconnect(users, ircServer.getNewSocket(), authData, authMap, "", channels);
						ircServer.getBuffer()[0] = '\0';
						continue;
					}

					password.clear();
					password = "";
					std::size_t passStart = passPos + 5, passEnd = 0;
					int i = passPos;
					for (; i < 1024; i++)
					{
						if (newBuffer[i] == '\n' && newBuffer[i - 1] == '\r')
						{
							passEnd = i - 2;
							break;
						}
					}
					passEnd = i - 1;
					for (std::size_t i = passStart; i < passEnd + 1; i++) {
                        password = newBuffer.substr(passStart, passEnd - passStart);
                    }
					if (password.compare(input.getPassword()) != 0)
					{
						WARNING("User trying to join with an invalid password");
						std::stringstream errReply;
						errReply << ":" << input.getHost() << " " << ERR_PASSWDMISMATCH << " :" << "Password incorrect"
								 << std::endl;
						send(ircServer.getNewSocket(), errReply.str().c_str(), errReply.str().size(), 0);
						ircServer.disconnect(users, ircServer.getNewSocket(), authData, authMap, "", channels);
						ircServer.getBuffer()[0] = '\0';
						continue;
					}
					users::t_authData userAuthData;
					for (std::vector<users::t_authData>::iterator it = authData.begin(); it != authData.end(); it++)
					{
						userAuthData = *it;
						if (userAuthData.sd == (std::size_t)sd) {
                            break;
                        }
					}

					if (nick.find("NICK") != std::string::npos)
					{
						if (users.getUser(nickname) != nullptr)
						{
							sendNickColissionError(ircServer.getNewSocket(), nickname, input.getHost());
                            ircServer.disconnect(users, sd, authData, authMap, "", channels);
							ircServer.disconnect(users, users.getUser(nickname)->getFd(), authData, authMap, users.getUser(nickname)->getNick(), channels);
							close(ircServer.getNewSocket());
							ircServer.getBuffer()[0] = '\0';
							WARNING("User " << nickname << " already exists. Killing both connection!");
							continue;
						} else
						{
							tempUser1 = users.createUser(nick.substr(5, strlen(ircServer.getBuffer()) - 5),
														 userAuthData.host,
														 userAuthData.port, sd);
						}
					}
					if (user.find("USER") != std::string::npos)
					{
						users::user *temp;

						if (users.getUser(nickname) != nullptr)
						{
							temp = users.getUser(nickname);
						} else
						{
							if (tempUser1 == nullptr)
							{
								msg.clear();
								ircServer.getBuffer()[0] = '\0';
								close(ircServer.getNewSocket());
								continue;
							}
							temp = users.getUserFromHost(tempUser1->getHost(), tempUser1->getPort());
						}
						users.updateUser(temp->getNick(), userAuthData.host, input.getHost(),
										 user.substr(user.find(":") + 1, user.size()));
						std::stringstream welcomemsg;
						welcomemsg << ":" << input.getHost() << " 001 " << temp->getNick()
								   << " :Welcome to the Internet Relay Network "
								   << temp->getNick() << "!" << temp->getUsername() << "@" << temp->getHost()
								   << std::endl;
						memset(ircServer.getBuffer(), '\0', 1024);
						int val = send(sd, welcomemsg.str().c_str(), welcomemsg.str().size(), 0);
						if (val > 0)
						{
							LOG("Welcome message sent to sd: " << sd << " user: " << users.getUser(sd)->getUsername());
							authMap[sd].second = true;
						} else
						{
							WARNING("Failed to send welcome message");
						}
					}

				}
			}
			//NOTE end of authentication
			if (FD_ISSET(sd, ircServer.getReadfds())) {
                memset(ircServer.getBuffer(), '\0', 1024);
				if ((valread = read(sd, ircServer.getBuffer(), 1024)) == 0) {
					if (authMap[sd].second == true) {
                        ircServer.disconnect(users, sd, authData, authMap, users.getUser(sd)->getNick(), channels);
                    }
                    else if(authMap[sd].second == false) {
                        ircServer.disconnect(users, sd, authData, authMap, "", channels);
                    }
                }
				else {
					if (authMap[sd].second == false) {
						if (!authMsgComplete(authMap[sd].first)) {
                            authMap.find(sd)->second.first += std::string(ircServer.getBuffer());
                        }
                        FD_CLR(sd, ircServer.getReadfds());
                        i--;
						continue;
					}
					ircServer.setPartialMsg(std::string(ircServer.getBuffer()), sd);
					while (ircServer.hasPartialMsg(sd) == true)
					{
						commands(users, sd, &channels, channelCount, ircServer.getBuffer(), input, ircServer, authData, authMap);
					}
				}
            }
        }
    }
	return (0);
}
