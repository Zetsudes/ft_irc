/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:07 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/20 14:14:34 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h> // pour5 le domaine (AF_INET cest a direr IPv4)
#include <sys/types.h> // le type ici sock_stream pour recevoir les messages dans lordre et ne pas perdre une partie du message
// IPPROTO_TCP  // pour TCP
#include <iostream>
#include <stdexcept> 
#include <fcntl.h>
#include <cerrno>

class Server 
{
    private:
        std::string password;
		int server_fd;

    public:
        Server();
        Server(Server const &src);
        Server &operator=(Server const &other);
        ~Server();

		void createSocket();
		void Server::serverLoop();
        
    
};

#endif