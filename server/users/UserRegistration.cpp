#include "UserRegistration.hpp"
#include "../debug/debug.hpp"


users::UserRegistration::UserRegistration() {

}

users::UserRegistration::~UserRegistration() {
    for (std::vector<user*>::iterator it = Users.begin(); it != Users.end(); it++)
    {
        delete(*it);
    }
    Users.clear();
}

users::user* users::UserRegistration::createUser(std::string nick, std::string host, std::size_t port, int fd) {
    if (getUser(nick) != nullptr) {
        return (nullptr);
    }
    Users.push_back(new user(nick));
    Users[Users.size() - 1]->setHost(host);
    Users[Users.size() - 1]->setPort(port);
    Users[Users.size() - 1]->setFd(fd);
    LOG("USER [" << nick << "] Created!");
    return (Users[Users.size() -1]);
}

std::size_t users::UserRegistration::updateUser(std::string oldNick, std::string newNick) {
    if (getUser(oldNick) != nullptr) {
        return (ERR_NICKCOLISSION);
    }
    user* user = getUser(oldNick);
    user->setNick(newNick);
    return (NO_ERR);
}

std::size_t users::UserRegistration::updateUser(std::string nick, std::string host, std::string serverName, std::string realName) {
    user* user = getUser(nick);
    user->setHost(host);
    user->setServerName(serverName);
    user->setRealName(realName.substr(0, realName.size() - 2));
    user->setUsername(nick);
    LOG("Updated user test with " << user->getHost() << " " << user->getServerName() << " " << user->getRealName());
    return (NO_ERR);
}

users::user* users::UserRegistration::getUser(std::string nick) {
    for (std::vector<user*>::iterator it = Users.begin(); it != Users.end(); it++)
    {
    	user *temp = *it;
        if (temp->getNick() == nick) {
            return (temp);
        }
    }
    return (nullptr);
}

users::user* users::UserRegistration::getUser(int fd)
{
	for (std::vector<user*>::iterator it = Users.begin(); it != Users.end(); it++)
    {
    	user *temp = *it;
        if (temp->getFd() == fd) {
            return (temp);
        }
    }
    return (nullptr);
}

users::user* users::UserRegistration::getUserFromHost(std::string host, std::size_t port) {

    for (std::vector<user*>::iterator it = Users.begin(); it != Users.end(); it++)
    {
    	user *temp = *it;
        if (temp->getHost() == host && temp->getPort() == port) {
            return (temp);
        }
    }
    return (nullptr);
}

std::size_t  users::UserRegistration::destroyUser(user *us) {
    if (us == nullptr) {
        ERROR("Unable to destroy user!");
        return (1);
    }
    std::vector<user*>::iterator it;
    for(it = Users.begin(); it != Users.end(); it++)
    {
        user *temp = *it;
        if (temp->getNick() == us->getNick()) {
            LOG("Destroying: " << temp->getNick());
            delete(temp);
            break;
        }
    }
    Users.erase(it);
    return (0);
}
