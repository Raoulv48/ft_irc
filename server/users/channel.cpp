#include "channel.hpp"
#include "../debug/debug.hpp"


Channel::Channel(std::string channelName, std::string channelKey, std::size_t channelId, std::string nickName, std::string opSet)
{
	setChannel(channelName);
	setOperator(nickName, opSet);
	setChannelId(channelId);
	if (channelKey.find("x") != std::string::npos && _nickNameOperator.size() == 1) {
        _channelKey = channelKey;
    }
	_topicMessage = "no topic message is been set";

}

Channel::~Channel()
{

}

void Channel::setChannel(std::string channelName)
{
	_channelName = channelName;
}

std::string Channel::getChannel() const
{
	return (_channelName);
}

void Channel::setChannelId(std::size_t channelId)
{
	_channelId = channelId;
}

std::size_t Channel::getChannelId()
{
	return (_channelId);
}

void Channel::setChannelKey(std::string channelKey)
{
	_channelKey = channelKey;
}

std::string Channel::getChannelKey()
{
	return (_channelKey);
}

bool Channel::isInChannel(std::string nickName)
{
    if (nickName.empty()) {
        return (false);
    }
    if (!_nickNameOperator.count(nickName)) {
        return (false);
    }
    return (true);
}

void Channel::setOperator(std::string nickName, std::string set)
{
	if (!isInChannel(nickName))
	{
		if (set.find("-o") != std::string::npos)
		{
			_nickNameOperator.insert(std::make_pair(nickName, false));
			LOG("op rights set to false for " << nickName);
		}
		else if (set.find("+o") != std::string::npos)
		{
			_nickNameOperator.insert(std::make_pair(nickName, true));
			LOG("op rights set to true for " << nickName);
		}
	}
	else
	{
		if (set.find("-o") != std::string::npos)
		{
			_nickNameOperator.find(nickName)->second = false;
			LOG("op rights set to false for " << nickName);
		}
		else if (set.find("+o") != std::string::npos)
		{
			_nickNameOperator.find(nickName)->second = true;
			LOG("op rights set to true for " << nickName);
		}
	}
}

bool Channel::getOperator(std::string nickName)
{
	if (_nickNameOperator.count(nickName) != 0)
	{
		if (_nickNameOperator.find(nickName)->second == true) {
            return (true);
        }
	}
	return (false);
}

void Channel::setTopicMessage(std::string topicMessage)
{
	_topicMessage = topicMessage;
}

std::string Channel::getTopicMessage()
{
	return (_topicMessage);
}
