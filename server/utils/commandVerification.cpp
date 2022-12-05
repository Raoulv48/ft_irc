#include "commandVerification.hpp"
#include <sstream>
#include <sys/socket.h>
#include "../debug/debug.hpp"
void sendFailMsg(int sd, std::string nickname)
{
	std::stringstream msg;
	// code 421 ERR_UNKOWNCOMMAND
	msg << ":" << nickname << " 421 " << "*"
		<< " :Unkown command"
		<< std::endl;
	send(sd, msg.str().c_str(), msg.str().length(), 0);
}

bool authVerificationPass(std::string msg)
{
    if (msg.find("PASS ") != 0)
	{
        return false;
	}
    
    std::string temp = msg.substr(5);
    int i = 0;
    while (temp[i] != '\0' && temp[i] != '\n')
	{
        i++;
	}
    if (temp[i] != '\n')
	{
        return false;
	}
    if (temp[i-1] != '\r')
	{
        return false;
	}
    if (i < 2)
	{
        return false;
	}
    return true;
}

bool authVerificationNick(std::string msg)
{
    if (msg.find("NICK ") != 0)
	{
        return false;
	}
    
    std::string temp = msg.substr(5);
    int i = 0;
    while (temp[i] != '\0' && temp[i] != '\n')
	{
        i++;
	}
    if (temp[i] != '\n')
	{
        return false;
	}
    if (temp[i-1] != '\r')
	{
        return false;
	}
    if (i < 2)
	{
        return false;
	}
    return true;
}

bool authVerificationUser(std::string msg)
{
    if (msg.find("USER ") != 0)
	{
        return false;
	}
    
    // User check
    std::string temp = msg.substr(5);
    int i = 0;
    while (temp[i] != '\0' && temp[i] != ' ')
	{
        i++;
	}
    if (temp[i] != ' ')
	{
        return false;
	}
    if (i < 1)
	{
        return false;
	}
    
    // Alternate user check
    temp = temp.substr(i);
    if (temp[0] != ' ')
	{
        return false;
	}
    temp = temp.substr(1);
    i = 0;
    while (temp[i] != '\0' && temp[i] != ' ')
	{
        i++;
	}
    if (temp[i] != ' ')
	{
        return false;
	}
    if (i < 1)
	{
        return false;
	}

    // IP check 
    temp = temp.substr(i);
    if (temp[0] != ' ')
	{
        return false;
	}
    temp = temp.substr(1);
    
    i = 0;
    while (temp[i] != ' ' && temp[i] != '\0')
	{
        i++;
	}
    if (i == 0 || temp[i] != ' ')
	{
        return false;
	}
	//End Ipcheck
	temp = temp.substr(i);

    i = 0;
    if (temp[i] != ' ')
	{
        return false;
	}
    temp = temp.substr(1);
    if (temp[i] != ':')
	{
        return false;
	}
    temp = temp.substr(1);

    i = 0;
    while (temp[i] != '\0' && temp[i] != '\n')
	{
        i++;
	}
    
    if (temp[i] != '\n' || i < 1 || temp[i-1] != '\r' || i < 2)
	{
        return false;
	}

    return true;
}

bool authVerification(std::string msg)
{

	std::string split[4];
    std::string temp;

    // First split
    if (msg.find("\r\n") == std::string::npos)
	{
        return false;
	}
    split[0] = msg.substr(0, msg.find("\r\n") + 2);
    if (split[0] != "CAP LS\r\n")
	{
        return (false);
	}

    
    // Second split
    temp = msg.substr(msg.find("\r\n") + 2);
    
    if (temp.find("\r\n") == std::string::npos)
	{
        return false;
	}
    split[1] = temp.substr(0, temp.find("\r\n") + 2);
    if (split[1].find("PASS ") != 0)
	{
        return false;
	}
    

    //Third split
    temp = temp.substr(temp.find("\r\n") + 2);

    if (temp.find("\r\n") == std::string::npos)
	{
        return false;
	}
    split[2] = temp.substr(0, temp.find("\r\n") + 2);
    if (split[2].find("NICK ") != 0)
	{
        return false;
	}



    // Fourth split
    temp = temp.substr(temp.find("\r\n") + 2);

    if (temp.find("\r\n") == std::string::npos)
	{
        return false;
	}
    split[3] = temp.substr(0, temp.find("\r\n") + 2);
    if (split[3].find("USER ") != 0)
	{
        return false;
	}

    
    // ALL CHECKS
    if (authVerificationPass(split[1]) == false)
	{
        return false;
	}
    if (authVerificationNick(split[2]) == false)
	{
        return false;
	}
    if (authVerificationUser(split[3]) == false)
	{
        return false;
	}

    return (true);
}

bool joinVerification(std::string msg)
{
	if (msg.size() > 512)
	{
		ERROR("JOIN: Msg size is too long");
		return (false);
	}
	std::size_t pos = 0;
	if ((pos = msg.find("#")) == std::string::npos) // NOTE: check to see if we find the start of the channel;
	{
		ERROR("JOIN: Couldn't find start of channel");
		return (false);
	}
	pos++;
	if (pos >= msg.size() - 1) // NOTE: channel name is missing
	{
		ERROR("JOIN: Channel name is not present");
		return (false);
	}
	for (; pos < msg.size() - 1; pos++) // NOTE: check for channel name validity
	{
		if (!std::isalnum(msg[pos]) && msg[pos] != ' ') // NOTE: ignore space so we can check key validity in one go if needed
		{
			ERROR("JOIN: Channel name is invalid");
			return (false);
		}
	}
	return (true);
}
bool kickVerification(std::string msg)
{
	if (msg.size() > 512)
	{
		ERROR("KICK: Msg size is too long");
		return (false);
	}
	bool chanFound = false, nickFound = false;
	for (std::size_t i = 0; i < msg.size() - 1; i++)
	{
		if (msg[i] == '#') // NOTE: Found start of channel
		{
			i++;
			while(i < msg.size() - 1 && msg[i] != ' ')
			{
				if (!std::isalnum(msg[i]))
				{
					ERROR("KICK: invalid format");
					return (false);
				}
				i++;
			}
			chanFound = true;
			continue;
		}
		if (chanFound == true && nickFound == false)
		{
			if (msg[i] == ' ') // NOTE: Go for message
			{
				i++;
				if (msg[i] != ':')
				{
					ERROR("KICK: Msg is missing");
					return (false);
				}
				nickFound = true;
			}
		}
		else if (chanFound == true && nickFound == true)
		{
			if (!std::isprint(msg[i])) // NOTE: check msg validity
			{
				ERROR("KICK: Msg is invalid");
				return (false);
			}
		}
	}
	if (chanFound == false || nickFound == false || msg.find(":") == std::string::npos)
	{
		ERROR("KICK: Message is incorrectly formatted");
		return (false);
	}
	return (true);
}
bool modeVerification(std::string msg)
{
	if (msg.size() > 512 || msg.size() < 7)
	{
		ERROR("MODE: Msg is invalid size");
		return (false);
	}
	std::size_t firstSpace = msg.find(" "); // NOTE: start to check if its not just MODE #Chan sent when joining channel
	if (firstSpace == std::string::npos || msg[firstSpace + 1] != '#')
	{
		ERROR("MODE: Invalid format");
		return (false);
	}
	firstSpace++;
	for (; firstSpace < msg.size() - 1; firstSpace++)
	{
		if (msg[firstSpace] == ' ')
			break;
	}
	if (firstSpace == msg.size() - 1)
	{
		return (true);
	} // NOTE: end of check
	bool isPlusK = false;
	for (std::size_t counter = 0; counter < msg.size() - 4; counter++)
	{
		if (msg[counter] == ' ' && msg[counter + 1] == '+' && msg[counter + 2] == 'k' && msg[counter + 3] == ' ')
		{
			isPlusK = true;
			break;
		}
	}
	if (isPlusK == true) // NOTE: check for +k operations in here
	{
		bool chanFound = false, keyFound = false;
		for (std::size_t i = 0; i < msg.size() - 1; i++)
		{
			if (msg[i] == '#') // NOTE: Found start of channel
			{
				i++;
				while(i < msg.size() - 1 && msg[i] != ' ')
				{
					if (!std::isalnum(msg[i]))
					{
						ERROR("MODE: Channel name is invalid");
						return (false);
					}
					i++;
				}
				chanFound = true;
			}
			if (chanFound == true && keyFound == false)
			{
				if (msg[i + 1] != '+' && msg[i + 2] != 'k') //NOTE: check if channelname is valid
				{
					ERROR("MODE: operation is invalid " << msg << "  " << i << msg[i + 1] << " " << msg[i + 2]);
					return (false);
				}
				i += 3;
				if (msg[i] == ' ') // NOTE: if space is found key starts
				{
					if (i == msg.size()) // NOTE: if i is already msg size then the key is missing
					{
						ERROR ("MODE: Key is missing");
						return (false);
					}
					keyFound = true;
				}
			}
			else if (chanFound == true && keyFound == true)
			{
				if (!std::isalnum(msg[i])) // NOTE: check key validity
				{
					ERROR("MODE: Key is invalid [" << msg[i] << "]");
					return (false);
				}
			}
		}
	}
	else // NOTE: check for #channel in here
	{
		bool chanFound = false, opFound = false, nickFound = false, opCorrect = false;
		for (std::size_t i = 0; i < msg.size() - 1; i++)
		{
			if (msg[i] == '#') // NOTE: Found start of channel
			{
				if (i == msg.size()) // NOTE: if i is already message size then msg is per definition incorrect
				{
					ERROR("MODE: Channel name is missing");
					return (false);
				}
				chanFound = true;
				continue;
			}
			if (chanFound == true && opFound == false && nickFound == false)
			{
				if (!std::isalnum(msg[i]) && msg[i] != ' ') //NOTE: check if channelname is valid
				{
					ERROR("MODE: Channel name is invalid");
					return (false);
				}
				if (msg[i] == ' ') // NOTE: if space is found op assigment starts
				{
					if (i == msg.size()) // NOTE: if i is already msg size then the rest is missing
					{
						ERROR("MODE: Missing command");
						return (false);
					}
					opFound = true;
				}
			}
			else if (chanFound == true && opFound == true && nickFound == false)
			{
				if (i == msg.size() || i + 1 == msg.size()) // NOTE: if size does not contain enough room for + or - o then its invalid
				{
					ERROR("MODE: Operation is invalid");
					return (false);
				}
				if ((msg[i] == '+' || msg[i] == '-') && msg[i + 1] == 'o') // NOTE: check if its +o or -o
				{
					opCorrect = true;
					i++;
					continue;
				}
				else if (opCorrect == false)
				{
					ERROR("MODE: Invalid operation")
					return (false);
				}
				if (opCorrect == true && msg[i] == ' ') // NOTE: Checking to make sure we have a valid command at the end of operator command
				{
					nickFound = true;
				}
			}
			else if (chanFound == true && opFound == true && nickFound == true)
			{
				if (!std::isalnum(msg[i])) // NOTE: Check validity of nickname
				{
					ERROR("MODE: Nickname is invalid");
					return (false);
				}
			}
		}
	}
	return (true);
}
bool pingVerification(std::string msg)
{
	std::size_t pos = 0;
	if (msg.size() > 512)
	{
		ERROR("PING: Msg size is too long");
		return (false);
	}
	if ((pos = msg.find(" ")) == std::string::npos) // NOTE: check for start of servername
	{
		ERROR("PING: Couldn't find start of servername");
		return (false);
	}
	pos++;
	if (pos >= msg.size() - 1) // NOTE: servername is not present
	{
		ERROR("PING: Servername is not present");
		return (false);
	}
	for (; pos < msg.size() - 1; pos++)
	{
		if (!std::isalnum(msg[pos])) // NOTE: check servername validity
		{
			ERROR("PING: Servername invalid");
			return (false);
		}
	}
	return (true);
}
bool nickVerification(std::string msg)
{
	std::size_t pos = 0;
	if (msg.size() > 512)
	{
		ERROR("NICK: Msg size is too long");
		return (false);
	}
	if ((pos = msg.find(" ")) == std::string::npos) // NOTE: check for start of nickname
	{
		ERROR("NICK: Couldn't find start of nickname");
		return (false);
	}
	pos++;
	if (pos >= msg.size() - 1) // NOTE: nickname is missing
	{
		ERROR("NICK: Nickname is not present");
		return (false);
	}
	for (; pos < msg.size() - 1; pos++)
	{
		if (!std::isalnum(msg[pos])) // NOTE: Check nickname validity
		{
			ERROR("NICK: Nickname is invalid");
			return (false);
		}
	}
	return (true);
}
bool partVerification(std::string msg)
{
	if (msg.size() > 512)
	{
		ERROR("PART: Msg is too long");
		return (false);
	}
	std::size_t pos1 = 0, pos2 = 0;
	if ((pos1 = msg.find("#")) == std::string::npos) // NOTE: check for start of channelname
	{
		ERROR("PART: Couldn't find start of channel name");
		return (false);
	}
	pos1++;
	if (pos1 >= msg.size() - 1) // NOTE: channelname is missing
	{
		ERROR("PART: Channel name is missing");
		return (false);
	}
	for (; msg[pos1] != ' ' && pos1 < msg.size() - 1; pos1++)
	{
		if (!std::isalnum(msg[pos1])) // NOTE: check channelname validity
		{
			ERROR("PART: Channelname is invalid");
			return (false);
		}
	}
	if ((pos2 = msg.find(":")) != std::string::npos) // NOTE: check if msg is present
	{
		for (; pos2 < msg.size() - 1; pos2++) // NOTE: check msg validity
		{
			if (!std::isprint(msg[pos2]))
			{
				ERROR("PART: Msg is invalid");
				return (false);
			}
		}
	}
	return (true);
}
bool privmsgVerifictation(std::string msg)
{
	std::size_t pos1 = 0, pos2 = 0;
	if (msg.size() > 512)
	{
		ERROR("PRIVMSG: Msg size is too long");
		return (false);
	}
	if ((pos1 = msg.find(" ")) == std::string::npos) // NOTE: find user/channel name
	{
		ERROR("PRIVMSG: Coudln't find start of user/channel name");
		return (false);
	}
	pos1++;
	if (pos1 >= msg.size() - 1) // NOTE: size is invalid
	{
		ERROR("PRIVMSG: Incorrectly formated");
		return (false);
	}
	for (; msg[pos1] != ' '; pos1++) // NOTE: check user/channel name validity
	{
		if (!std::isalnum(msg[pos1]) && msg[pos1] != '#')
		{
			ERROR("PRIVSMG: User/channel name is invalid");
			return (false);
		}
	}
	if ((pos2 = msg.find(":")) != std::string::npos) // NOTE: check for message existance
	{
		if (msg[pos2 - 1] != ' ')
		{
			ERROR("PRIVMSG: Msg is not preceded by a space");
			return (false);
		}
	}
	else
	{
		ERROR("PRIVMSG: Msg is missing");
		return (false);
	}
	pos2++;
	if (pos2 >= msg.size() - 1) // NOTE: msg is missing
	{
		ERROR("PRIVMSG: Msg is missing");
		return (false);
	}
	for(; pos2 != msg.size() - 1; pos2++) // NOTE: check message validity
	{
		if (!std::isprint(msg[pos2]))
		{
			ERROR("PRIVMSG: Msg is invalid");
			return (false);
		}
	}
	return (true);
}
bool noticeVerification(std::string msg)
{
	std::size_t pos1 = 0, pos2 = 0;
	if (msg.size() > 512)
	{
		ERROR("NOTICE: Msg size is too long");
		return (false);
	}
	if ((pos1 = msg.find(" ")) == std::string::npos) // NOTE: find user/channel name
	{
		ERROR("NOTICE: Coudln't find start of user/channel name");
		return (false);
	}
	pos1++;
	if (pos1 >= msg.size() - 1) // NOTE: size is invalid
	{
		ERROR("NOTICE: Incorrectly formated");
		return (false);
	}
	for (; msg[pos1] != ' '; pos1++) // NOTE: check user/channel name validity
	{
		if (!std::isalnum(msg[pos1]) && msg[pos1] != '#')
		{
			ERROR("NOTICE: User/channel name is invalid");
			return (false);
		}
	}
	if ((pos2 = msg.find(":")) != std::string::npos) // NOTE: check for message existance
	{
		if (msg[pos2 - 1] != ' ')
		{
			ERROR("NOTICE: Msg is not preceded by a space");
			return (false);
		}
	}
	else
	{
		ERROR("NOTICE: Msg is missing");
		return (false);
	}
	pos2++;
	if (pos2 >= msg.size() - 1) // NOTE: msg is missing
	{
		ERROR("NOTICE: Msg is missing");
		return (false);
	}
	for(; pos2 != msg.size() - 1; pos2++) // NOTE: check message validity
	{
		if (!std::isprint(msg[pos2]))
		{
			ERROR("NOTICE: Msg is invalid");
			return (false);
		}
	}
	return (true);
}
bool topicVerification(std::string msg)
{
	std::size_t pos1 = 0, pos2 = 0;
	if (msg.size() > 512)
	{
		ERROR("TOPIC: Msg size is too long");
		return (false);
	}
	if ((pos1 = msg.find("#")) == std::string::npos) // NOTE: Check if channel start exists
	{
		ERROR("TOPIC: Coudln't find start of channel name");
		return (false);
	}
	pos1++;
	if (pos1 >= msg.size() - 1) // NOTE: channel is missing
	{
		ERROR("TOPIC: Channel name is not present");
		return (false);
	}
	for (; msg[pos1] != ' '; pos1++)
	{
		if (!std::isalnum(msg[pos1])) // NOTE: Check channel validity
		{
			ERROR("TOPIC: Channel name is invalid");
			return (false);
		}
	}
	if ((pos2 = msg.find(":")) == std::string::npos)
	{
		ERROR("TOPIC: Couldn't find message");
		return (false);
	}
	if (pos2 + 1 >= msg.size() - 1 || msg[pos2 - 1] != ' ') // NOTE: either message is missing or not preceded by a space
	{
		ERROR("TOPIC: Message is missing or formated incorrectly");
		return (false);
	}
	pos2++;
	for (; pos2 < msg.size() - 1; pos2++) // NOTE: check message validity
	{
		if (!std::isprint(msg[pos2]))
		{
			ERROR("TOPIC: Message is invalid");
			return (false);
		}
	}
	return (true);
}
