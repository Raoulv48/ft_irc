SERVER = ircserv
CLIENT = client

SRCS = server/main.cpp server/utils/commandUtils.cpp server/commands/part.cpp server/commands/join.cpp server/commands/privmsg.cpp server/commands/pingpong.cpp server/commands/nick.cpp server/commands/mode.cpp server/commands/kick.cpp server/inputparser/inputparser.cpp server/users/user.cpp server/users/UserRegistration.cpp server/users/operator.cpp server/users/topic.cpp server/replyMessage.cpp server/users/channel.cpp server/utils/MessageVerification.cpp server/commands.cpp server/server.cpp server/utils/commandVerification.cpp server/commands/notice.cpp

OBJS = $(SRCS:.cpp=.o)

MAX_CLIENTS ?= 30

FLAGS ?= -Wall -Werror -Wextra

all: $(SERVER)

$(SERVER):
	$(MAKE) -j $(OBJS)
	mkdir -p logs
	clang++ -o $@ $(OBJS)  -Wall  -Wextra -std=c++98 -pedantic -D MAX_CLIENTS=$(MAX_CLIENTS) $(FLAGS)

%.o: %.cpp
	clang++ -o $@ -c $^ -Wall  -Wextra -std=c++98 -pedantic -fPIC -g -D MAX_CLIENTS=$(MAX_CLIENTS) $(FLAGS)

bonus:
	$(MAKE) -C bot/ all
	cp bot/bonusbot .

clean:
	-rm -f $(OBJS)
	$(MAKE) -C bot/ clean

fclean: clean
	-rm -f $(SERVER) bonusbot
	$(MAKE) -C bot/ fclean

re: fclean all

.PHONY: server bot logs
