#include "server.hpp"
#include <sstream>

server::server(std::size_t port, std::string password, InputParser &input) : _opt(1), _currUsers(0), _port(port), _input(input)
{
    (void)password;
	_hostName = gethostname(_host, sizeof(_host));
	this->_serverSetup();
}

server::~server()
{

}

void server::_serverSetup() {
	std::size_t i;

	for (i = 0; i < MAX_CLIENTS; i++) {
        _clientSocket[i] = 0;
    }

	if ((_masterSocket = socket(AF_INET, SOCK_STREAM,0)) == 0) {
		CRITICAL("Failed to create master socket");
	}

	// set master socket to allow multiple connections
	if(setsockopt(_masterSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&_opt, sizeof(_opt)) < 0) {
		CRITICAL("Failed to set master socket to allow mutliple connections");
	}

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);



	if (bind(_masterSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
		CRITICAL("Failed to bind port");
	}

	if (listen(_masterSocket, 3) < 0) {
		CRITICAL("Failed to listen!");
	}

	_addrlen = sizeof(_address);

	LOG("Server is now listening on: " << _host << ":" << ntohs(_address.sin_port));
}


void server::Select() {
	FD_SET(_masterSocket, &_readfds);
	_maxSd = _masterSocket;
	std::size_t i;
	for (i = 0; i < MAX_CLIENTS; i++)
	{
		_sd = _clientSocket[i];

		if (_sd > 0)
		{
			FD_SET(_sd, &_readfds);
		}

		if (_sd > _maxSd) {
            _maxSd = _sd;
        }
	}

	_activity = select(_maxSd + 1, &_readfds, NULL, NULL, NULL);

	if (_activity < 0) {
		CRITICAL("Select Error!");
	}
}

bool server::Accept(users::UserRegistration &users, std::vector<users::t_authData> &authData, std::map<int, std::pair<std::string, bool> > &authMap, std::map<std::string, Channel*> &channels)
{

	std::size_t i;
	if ((_newSocket = accept(_masterSocket, (struct sockaddr *)&_address, (socklen_t*)&_addrlen)) < 0) {
        ERROR("Error occurred on accept");
    }

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (_clientSocket[i] == 0) {
			_clientSocket[i] = _newSocket;
			fcntl(_clientSocket[i], F_SETFL, O_NONBLOCK);
			break;
		}
	}
	_currUsers++;


	if (_currUsers > MAX_CLIENTS) {
        std::stringstream disconnectMsg;
        disconnectMsg << ":" << _address.sin_addr.s_addr << " 005 " << "*"
                      << " :Server is full!"
                      << std::endl;
        send(_newSocket, disconnectMsg.str().c_str(), disconnectMsg.str().length(), 0);

        this->disconnect(users, _newSocket, authData, authMap, "", channels);
    }
	return (true);
}


char *server::getBuffer() {
	return _buffer;
}

int server::getNewSocket() {
	return _newSocket;
}

int server::getMasterSocket() {
	return _masterSocket;
}

int server::getClientSocket(int clientID) {
	return _clientSocket[clientID];
}


fd_set *server::getReadfds() {
	return &_readfds;
}

struct sockaddr_in *server::getAddress() {
	return &_address;
}

std::string server::getPartialMessage(int sd)
{
	std::string fullMsg = "";
			for (std::size_t i = 0; i <= _partialMsg.find(sd)->second.size(); i++)
			{
				if (_partialMsg.find(sd)->second[i] == '\n' && _partialMsg.find(sd)->second[i - 1] == '\r') {
					fullMsg = _partialMsg.find(sd)->second.substr(0, i);
					if (i + 1 == _partialMsg.find(sd)->second.size())
					{
						_partialMsg.find(sd)->second.clear();
						break;
					}
					std::string remainder = _partialMsg.find(sd)->second.substr(i + 1, _partialMsg.find(sd)->second.size() - (i + 1));
					_partialMsg.find(sd)->second.clear();
					_partialMsg.find(sd)->second = remainder;
					break;
				}
			}

	return (fullMsg);
}

bool server::hasPartialMsg(int sd)
{
	if (_partialMsg.count(sd) != 0) {
		if (incMsgComplete(_partialMsg.find(sd)->second))
		{
			return (true);
		}
	}
	return (false);
}

void server::setPartialMsg(std::string buffer, int sd)
{
    if (_partialMsg.count(sd) == 0) {
        _partialMsg.insert(std::make_pair(sd, buffer));
    }
    else
    	_partialMsg.find(sd)->second = _partialMsg.find(sd)->second + buffer;

}

void server::disconnect(users::UserRegistration &users, int sd, std::vector<users::t_authData> &authData, std::map<int, std::pair<std::string, bool> > &authMap, std::string nickname, std::map<std::string, Channel*> &channels)
{
	getpeername(sd, (struct sockaddr *) &_address,
				(socklen_t * ) &_addrlen);
	LOG("Host disconnected, ip " << inet_ntoa(_address.sin_addr) << ", port " << ntohs(_address.sin_port) << ", sd: " << sd);
	std::stringstream newmsg;
	if (!nickname.empty()) { //NOTE Only delete the user if he has a nickname, otherwise he isn't registered
        for (std::size_t i = 0; i < users.getUser(sd)->_channelName.size(); i++) {
            std::string partMsg = "PART " + users.getUser(sd)->_channelName[i];
            partEntry(users, sd, partMsg, _buffer, channels, _input);
        }
		users.destroyUser(users.getUser(sd));
    }
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (sd == _clientSocket[i])
		{
			_clientSocket[i] = 0;
			break;
		}
	}
	FD_CLR(sd, &_readfds);
	for (std::vector<users::t_authData>::iterator it = authData.begin(); it != authData.end(); it++)
	{
		users::t_authData temp = *it;
		if ((int)temp.sd == sd)
		{
			authData.erase(it);
			break;
		}
	}
	for (std::map<int, std::pair<std::string, bool> >::iterator it = authMap.begin(); it != authMap.end(); it++)
	{
		int temp = it->first;
		if (temp == sd)
		{
			authMap.erase(it);
			break;
		}
	}
    FD_ZERO(&_readfds);
	close(sd);
	_currUsers--;
}
