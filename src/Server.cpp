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
	else
	{
		std::string line(buffer, bytesRead);
		while (!line.empty() && (line[line.length() - 1] == '\n' || line[line.length() - 1] == '\r'))
			line.erase(line.length() - 1);
		 std::cout << "Received from " << client_fd << ": " << line << std::endl;
		Parsing parse = Parsing::parse(line);
		Client* client = getClientByFd(client_fd);
		if (client)
		{
			CommandHandler handler(*this, *client);
			std::string cmd = parse.request;
			if (cmd == "PASS")
				handler.handlePass(parse);
			else if (cmd == "NICK")
				handler.handleNick(parse);
			else if (cmd == "USER")
				handler.handleUser(parse);
			else if (cmd == "PRIVMSG")
				handler.handlePrivmsg(parse);
			else if (cmd == "QUIT")
				handler.handleQuit(parse);
			// JOIN, PART, TOPIC, KICK,...
		}
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

const std::string& Server::getPassword() const {
	return _password;
}

void Server::announceQuit(Client& client, const std::string& reason)
{
	std::string msg = ":" + client.getNickname() + " QUIT :" + reason + "\r\n";
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->first != client.getFd())
			send(it->first, msg.c_str(), msg.size(), 0);
	}
}

void Server::removeClient(int fd)
{
	Client* client = getClientByFd(fd);
	if (!client)
		return;
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

