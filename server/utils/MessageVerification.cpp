#include "MessageVerification.hpp"
#include "../debug/debug.hpp"

bool authMsgComplete(std::string buffer)
{
	std::size_t crlfCounter = 0;

	for (std::size_t i = 0 ; i < buffer.size(); i++) {
        if (buffer[i] == '\n' && buffer[i - 1] == '\r') {
            crlfCounter++;
        }
        if (crlfCounter == 4) {
            return (true);
        }
	}
	return false;
}

bool incMsgComplete(std::string buffer)
{
	for (std::size_t i = 0; i < buffer.size(); i++)
	{
		if (buffer[i] == '\n' && buffer[i - 1] == '\r')
		{
			if (i + 1 != buffer.size())
			{
				continue;
			}
			else {
                return (true);
            }
		}
	}
	return (false);
}
