/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:25:33 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/23 13:25:34 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//ajouter constructeurs etc..

void Server::createSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_fd < 0)
		throw std::runtime_error("Error: Failed to create server socket");

	int reuseOptionOn = 1;
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseOptionOn, sizeof(reuseOptionOn)) == -1)
		throw std::runtime_error("Error: Failed to set socket options");

	struct sockaddr_in server_address = {0};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(6667);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if(bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
		throw std::runtime_error("Error: Failed to bind server socket");

	if(listen(server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: Could not listen");

	if(fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error: Failed to set socket options");
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
				throw std::runtime_error("accept() failed");
		}
		fcntl(client_fd, F_SETFL, O_NONBLOCK); pollfd clientPoll;
		clientPoll.fd = client_fd;
		clientPoll.events = POLLIN;
		clientPoll.revents = 0;
		connections.push_back(clientPoll);
		std::cout << "New client connected: " << client_fd << std::endl;
	}
}

void Server::readClientMessage(int client_fd)
{
	char buffer[1024] = {};
	int bytesRead = recv(client_fd, buffer, sizeof(buffer), 0);
	if (bytesRead <= 0)
	{
		close(client_fd);
		for (size_t i = 0; i < connections.size(); i++)
		{
			if (connections[i].fd == client_fd)
			{
				connections.erase(connections.begin() + i);
				break;
			}
		}
	}
	else
	{
		std::string reply = "Message received!\r\n";
		send(client_fd, reply.c_str(), reply.size(), 0);
	}
}

Client* Server::getClientByFd(int fd)
{
    std::map<int, Client>::iterator it = clients.find(fd);
    if (it != clients.end())
        return &(it->second);
    return NULL;
}