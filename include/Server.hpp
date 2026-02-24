/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:07 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/24 15:04:08 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h> // pour5 le domaine (AF_INET cest a direr IPv4)
#include <sys/types.h> // le type ici sock_stream pour recevoir les messages dans lordre et ne pas perdre une partie du message
// IPPROTO_TCP  // pour TCP
#include <map>
#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <cerrno>
#include <vector>
#include <poll.h>
#include <unistd.h>

class Client;
class Channel;

class Server
{
	private:
		int	server_fd;
		int _port;
		std::string _passsword;
		
		std::vector<pollfd>	connections;
		std::map<int, Client> clients;
		std::map<std::string, Channel> channels;
		
	public:
		Server(int port, const std::string& password);
		~Server();

		Client* getClientByFd(int fd);
		Client* getClientByNick(const std::string& nick);
		Channel* getChannel(const std::string& name);
		
		void	createSocket();
		void	serverLoop();
		void	acceptNewClient();
		void	readClientMessage(int client_fd);

		void removeClient(int fd);
};

