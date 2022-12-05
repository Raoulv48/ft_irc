#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <vector>

namespace users
{

typedef struct s_authData
{
	s_authData(){}
	s_authData(std::size_t _sd, std::string _host, std::size_t _port) : sd(_sd), host(_host), port(_port) {}
	std::size_t sd;
	std::string host;
	std::size_t port;
} t_authData;

	class user
	{
	public:
		user(std::string nickName);
		~user();
		void setUsername(std::string userName);
		std::string getUsername() const;
		void setNick(std::string nick);
		std::string getNick() const;
		void setHost(std::string host);
		std::string getHost() const;
		void setServerName(std::string serverName);
		std::string getServerName() const;
		void setRealName(std::string realName);
		std::string getRealName() const;
		void setPort(std::size_t port);
		std::size_t getPort() const;
		int addChannel(std::string channel);
		void setFd(int fd);
		int getFd() const;
		std::vector<std::string> _channelName;

	private:
		std::string _userName;
		std::string _nickName;
		std::string _serverName;
		std::string _host;
		std::size_t _port;
		std::string _realName;
		int _fd;
	};
}

#endif
