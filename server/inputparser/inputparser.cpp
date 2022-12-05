#include "inputparser.hpp"
#include <cstdlib>
#include "../debug/debug.hpp"


InputParser::InputParser(char **argv, int argc) : _host("CodamIRC")
{
	if (argc == 3) {
	    std::string password(argv[2]);
	    this->_port = std::atol(argv[1]);
	    this->_password = password;
    }
	this->_printInfo();
}

InputParser::~InputParser()
{

}

std::string InputParser::getPassword() const
{
	return (_password);
}

std::size_t InputParser::getPort() const
{
	return (_port);
}

const std::string InputParser::getHost() const
{
    return (_host);
}

void InputParser::_printInfo() const
{
	LOG( std::endl \
	<< "========= SERVER INFO ==========" << std::endl \
	<< "port: " << _port << std::endl \
	<< "password: " << _password << std::endl \
	<<"================================");
}
