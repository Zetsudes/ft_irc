*This project has been created as part of the 42 curriculum by zamohame, pmeimoun.*

# Project Name
ft_irc

### Description
The goal of ft_irc is to implement a TCP server that listens for incoming connections and can handle multiple clients simultaneously.
The server manages all communication between clients according to the IRC protocol.

### Instructions
To use the server, first compile it by running `make`. Then launch it with `./ircserv <PORT> <PASSWORD>`, replacing `<PORT>` with the port number the server should listen on and `<PASSWORD>` with the password clients must provide to connect. Clients can connect and interact with the server using IRC commands such as `PASS`, `NICK`, `USER`, `JOIN`, and `PRIVMSG` to log in, join channels, and send messages to other users.

### Resources
***TimGPT (or tcezard)*** 
Better than any AI, he was able to enlighten us about poll(), POLLIN/POLLOUT, communication between server and clients and the kernel amongst many other things. The exchange lasted an hour and not one nanosecond was wasted. His efforts shall not be in vain.


In ft_irc, the architecture is strictly client–server.
Each IRC client connects to the IRC server, sends commands, and receives responses. Clients never communicate directly with each other.
All messages go through the server.

### Testing with IRC Clients
nc localhost port 
irssi
Hexchat

