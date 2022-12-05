#ifndef INPUT_PARSER_HPP
# define INPUT_PARSER_HPP

#include <iostream>

class InputParser
{
public:
	InputParser(char **argv, int argc);
	~InputParser();

	std::string getPassword() const;
	std::size_t getPort() const;
	const std::string getHost() const;


private:
	std::string _password;
	std::size_t _port;
	const std::string _host;
	void _printInfo() const;
};

#endif
