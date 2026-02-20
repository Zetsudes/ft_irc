/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:07 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/20 13:24:42 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h> // pour5 le domaine (AF_INET cest a direr IPv4)
#include <sys/types.h> // le type ici sock_stream pour recevoir les messages dans lordre et ne pas perdre une partie du message
// IPPROTO_TCP  // pour TCP

class Server 
{
    private:
        std::string password;

    public:
        Server();
        Server(Server const &src);
        Server &operator=(Server const &other);
        ~Server();
        
    
};

#endif