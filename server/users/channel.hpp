#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <iostream>
#include <map>

class Channel
{
public:
	Channel(std::string channelName, std::string channelKey, std::size_t channelId, std::string nickName, std::string opSet);
	~Channel();

	void setChannel(std::string);
	std::string getChannel() const;
	void setChannelId(std::size_t channelId);
	std::size_t getChannelId();
	void setChannelKey(std::string channelKey);
	std::string getChannelKey();

	bool isInChannel(std::string nickName);
	void setOperator(std::string nickName, std::string set);
	bool getOperator(std::string nickName);

	void setTopicMessage(std::string topicMessage);
	std::string getTopicMessage();

public:
	std::string _channelKey;
	std::string _channelName;
	std::size_t _channelId;
	std::string _topicMessage;
	std::map<std::string, bool> _nickNameOperator;
};


#endif
