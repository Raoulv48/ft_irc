#ifndef SERVER_HPP
#define SERVER_HPP
#include "debug/debug.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sstream>
#include <fcntl.h>
#include "debug/debug.hpp"
#include "utils/commandUtils.hpp"
#include "users/topic.hpp"
#include <map>
#include "commands/part.hpp"
#include "commands/join.hpp"
#include "commands/privmsg.hpp"
#include "commands/pingpong.hpp"
#include "commands/nick.hpp"
#include "commands/mode.hpp"
#include "commands/kick.hpp"
#include "users/channel.hpp"
#include "utils/MessageVerification.hpp"

#ifndef MAX_CLIENTS
    #define MAX_CLIENTS 30
#endif

class server {

public:
	server(std::size_t port, std::string password, InputParser &input);
	~server();

	char *getBuffer();
	int getNewSocket();
	int getMasterSocket();
	int getClientSocket(int clientID);
	fd_set *getReadfds();

	struct sockaddr_in *getAddress();

	std::string getPartialMessage(int sd);
	bool hasPartialMsg(int sd);
	void setPartialMsg(std::string buffer, int sd);

	void disconnect(users::UserRegistration &users, int sd, std::vector<users::t_authData> &authData, std::map<int, std::pair<std::string, bool> > &authMap, std::string nickname, std::map<std::string, Channel*> &channels);
	void Select();
	bool Accept(users::UserRegistration &users, std::vector<users::t_authData> &authData, std::map<int, std::pair<std::string, bool> > &authMap, std::map<std::string, Channel*> &channels);

private:
	char _host[256], _buffer[1024];
	int _newSocket, _masterSocket, _clientSocket[MAX_CLIENTS], _activity, _maxSd, _opt, _sd, _hostName;
	struct sockaddr_in _address;
	int _addrlen;
	std::size_t _currUsers;

	std::size_t _port;
	fd_set _readfds;
    InputParser &_input;
    std::map<int, std::string> _partialMsg;

	void _serverSetup();


};


#endif
