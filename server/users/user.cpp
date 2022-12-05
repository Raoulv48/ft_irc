#include "user.hpp"

users::user::user(std::string nickName) : _nickName(nickName)
{

}

users::user::~user()
{

}

void	users::user::setUsername(std::string userName)
{
	_userName = userName;
}

std::string	users::user::getUsername() const
{
	return (_userName);
}

void	users::user::setHost(std::string host)
{
	_host = host;
}

std::string	users::user::getHost() const
{
	return (_host);
}

void 	users::user::setServerName(std::string serverName)
{
	_serverName = serverName;
}

std::string	users::user::getServerName() const
{
	return (_serverName);
}

void 	users::user::setRealName(std::string realName)
{
	_realName = realName;
}

std::string	users::user::getRealName() const
{
	return (_realName);
}

void users::user::setNick(std::string nick)
{
	_nickName = nick;
}

std::string users::user::getNick() const
{
	return (_nickName);
}

void users::user::setPort(std::size_t port)
{
	_port = port;
}

std::size_t users::user::getPort() const
{
	return (_port);
}

void users::user::setFd(int fd)
{
	_fd = fd;
}

int users::user::getFd() const
{
	return (_fd);
}

int users::user::addChannel(std::string channelName) {
    _channelName.push_back(channelName);
    return (0);
}
