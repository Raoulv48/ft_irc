#ifndef USERREGISTRATION_HPP
#define USERREGISTRATION_HPP
#include <iostream>
#include <vector>
#include "user.hpp"
#include "../numreplies.hpp"

#ifdef __linux__
	#define nullptr NULL
#endif

namespace users {

    class UserRegistration {
    public:
        UserRegistration();
        ~UserRegistration();
        user* createUser(std::string nick, std::string host, std::size_t port, int fd);
        std::size_t updateUser(std::string oldNick, std::string newNick);
        std::size_t updateUser(std::string nick, std::string host, std::string serverName, std::string realName);
        user* getUser(std::string nick);
		user* getUser(int fd);
        user* getUserFromHost(std::string host, std::size_t port);
        std::size_t destroyUser(user* us);

    private:
        std::vector<user*> Users;
    };

}

#endif
