# FT_IRC [![jlensing's 42 ft_irc Score](https://badge42.vercel.app/api/v2/cl1jwurq1001109l1q4s085aw/project/2362709)](https://github.com/JaeSeoKim/badge42)
* Written in C++98
* Compiled with -Wall -Werror -Wextra -std=c++98 -pedantic
* Works with IRSSI

## _How to clone and compile_
Simply run
```
git clone git@github.com:Axenth/ft_irc.git && cd ft_irc && make
```

If you wish to have more then the default 30 clients allowed you can do
```
make MAX_CLIENTS=XX
```

## _How to use_
```
./ircserv PORT PASSWORD
```
After that just hop on over to IRSSI and connect to the server.

Currently supports:

- Private messages
- Rooms
- Room operators
- Notice
- Changing your nickname
- Setting a topic
- Kicking users from a room
- Setting a room password

## credits
- Jordi Lensing (https://github.com/Axenth)
- Raoul Verschoor (https://github.com/Raoulv48)
- Isaac Donado (https://github.com/IsaacDonado-TomTom)
