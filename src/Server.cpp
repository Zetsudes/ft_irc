/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:25:33 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/03/05 11:46:13 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"


Server::Server(int port, const std::string& password)
    : _port(port), _password(password)
{
    createSocket();
}

Server::~Server()
{
    close(server_fd);
}

void Server::createSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_fd < 0)
		throw std::runtime_error("Error: Failed to create server socket");

	int reuseOptionOn = 1;
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseOptionOn, sizeof(reuseOptionOn)) == -1)
		throw std::runtime_error("Error: Failed to set socket options");

	struct sockaddr_in server_address;
	std::memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(this->_port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if(bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
		throw std::runtime_error("Error: Failed to bind server socket");

	if(listen(server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: Could not listen");

	if(fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error: Failed to set socket options");

	pollfd serverPoll;
    serverPoll.fd = server_fd;
    serverPoll.events = POLLIN;
    serverPoll.revents = 0;
    connections.push_back(serverPoll);

    std::cout << "Server listening on port " << _port << std::endl;
}

void Server::serverLoop()
{
	while (true)
	{
		if (poll(&connections[0], connections.size(), -1) == -1)
			throw std::runtime_error("Error: poll() failed");
		for (size_t i = 0; i < connections.size(); i++)
		{
			if (connections[i].revents & POLLIN)
			{
				if (connections[i].fd == server_fd)
					acceptNewClient();
				else
					readClientMessage(connections[i].fd);
			}

			if (connections[i].revents & POLLOUT)
            {
                Client* client = getClientByFd(connections[i].fd);
                if (client && !client->getBuffer().empty())
                {
                    int bytesSent = send(client->getFd(), client->getBuffer().c_str(), client->getBuffer().size(), 0);
                    if (bytesSent > 0)
                        client->getBuffer().erase(0, bytesSent);

                    if (client->getBuffer().empty())
                        connections[i].events &= ~POLLOUT;
                }
            }
		}
	}
}

void Server::acceptNewClient()
{
	while (true)
	{
		sockaddr_in client_address;
		socklen_t client_len = sizeof(client_address);
		int client_fd = accept(server_fd, (sockaddr*)&client_address, &client_len);
		if (client_fd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else
				throw std::runtime_error("Error: accept() failed");
		}
		if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
            throw std::runtime_error("Error: Failed to set client non-blocking");
		pollfd clientPoll;
		clientPoll.fd = client_fd;
		clientPoll.events = POLLIN;
		clientPoll.revents = 0;
		connections.push_back(clientPoll);
		clients.insert(std::make_pair(client_fd, Client(client_fd)));
		std::cout << "New client connected: " << client_fd << std::endl;
	}
}

void Server::readClientMessage(int client_fd)
{
	char buffer[1024] = {};
	int bytesRead = recv(client_fd, buffer, sizeof(buffer), 0);
	if (bytesRead <= 0)
	{
		std::cout << "Client disconnected: " << client_fd << std::endl;
		close(client_fd);
		for (size_t i = 0; i < connections.size(); i++)
		{
			if (connections[i].fd == client_fd)
			{
				connections.erase(connections.begin() + i);
				break;
			}
		}
		clients.erase(client_fd);
		return;
	}
	 Client* client = getClientByFd(client_fd);
    if (!client) return;

    client->appendToParseBuffer(std::string(buffer, bytesRead));

    std::string& raw = client->getParseBuffer();
    std::string::size_type pos;
    while ((pos = raw.find('\n')) != std::string::npos)
    {
        std::string line = raw.substr(0, pos);
        raw.erase(0, pos + 1);

        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        if (line.empty()) continue;

        std::cout << "Received from " << client_fd << ": " << line << std::endl;
        Parsing parse = Parsing::parse(line);
        CommandHandler handler(*this, *client);
        std::string cmd = parse.request;
        if (cmd == "PASS")       handler.handlePass(parse);
        else if (cmd == "NICK")  handler.handleNick(parse);
        else if (cmd == "USER")  handler.handleUser(parse);
        else if (cmd == "PRIVMSG") handler.handlePrivmsg(parse);
        else if (cmd == "QUIT")  handler.handleQuit(parse);
		else if (cmd == "JOIN")  handler.handleJoin(parse);
		else if (cmd == "PART")  handler.handlePart(parse);
		else if (cmd == "TOPIC")  handler.handleTopic(parse);
		else if (cmd == "KICK")  handler.handleKick(parse);
		else if (cmd == "INVITE")  handler.handleInvite(parse);
		else if (cmd == "MODE")  handler.handleMode(parse);
    }
}

Client* Server::getClientByFd(int fd)
{
    std::map<int, Client>::iterator it = clients.find(fd);
    if (it != clients.end())
        return &(it->second);
    return NULL;
}

Client* Server::getClientByNickname(const std::string& nick)
{
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.getNickname() == nick)
            return &(it->second);
    }
    return NULL;
}

Channel* Server::getChannel(const std::string& name)
{
    std::map<std::string, Channel>::iterator it = channels.find(name);
    if (it != channels.end())
        return &(it->second);
    return NULL;
}

Channel* Server::createChannel(const std::string &name)
{
	channels[name] = Channel(name);
	return &channels[name];
}

const std::string& Server::getPassword() const {
	return _password;
}

void Server::removeChannel(const std::string& name)
{
	channels.erase(name);
}

void Server::announceQuit(Client& client, const std::string& reason)
{
    std::string msg = ":" + client.getNickname() + " QUIT :" + reason + "\r\n";
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->first != client.getFd())
        {
            it->second.appendToBuffer(msg);
            handlePollout(it->second);
        }
    }
}

void Server::removeClient(int fd)
{
	Client* client = getClientByFd(fd);
	if (!client)
		return;
	std::vector<std::string> toRemove;
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		it->second.removeClient(client);
		if (it->second.memberCount() == 0)
			toRemove.push_back(it->first);
	}
	for (size_t i = 0; i < toRemove.size(); i++)
		channels.erase(toRemove[i]);
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (connections[i].fd == fd)
		{
			connections.erase(connections.begin() + i);
			break;
		}
	}
	close(fd);
	clients.erase(fd);
}

void Server::handlePollout(Client& client)
{
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (connections[i].fd == client.getFd())
		{
			connections[i].events |= POLLOUT;
			break;
		}
	}
}

void Server::sendWelcome(Client& client)
{
	std::string nick = client.getNickname();
	std::string msg;

	msg = ":ircserv 001 " + nick + " :Welcome to the IRC server " + nick + "\r\n";
	client.appendToBuffer(msg);

	msg = ":ircserv 002 " + nick + " :Your host is ircserv\r\n";
	client.appendToBuffer(msg);

	msg = ":ircserv 003 " + nick + " :This server was created today\r\n";
	client.appendToBuffer(msg);

	msg = ":ircserv 004 " + nick + " ircserv 1.0 o o\r\n";
	client.appendToBuffer(msg);
	handlePollout(client);
}

