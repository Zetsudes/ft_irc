/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:11 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/20 14:13:14 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal.h>

class Client
{
    private:
        std::string nickName;
        std::string userName;
        std::string realName;
        int socket_fd;
    
    public:
        Client();
        Client(Client const &src);
        Client &operator=(Client const &other);
        ~Client();

        void setNickName(const std::string& str);
        void setUserName(const std::string& str);
        void setRealName(const std::string& str);

        std::string getNickName() const;
        std::string getUserName() const;
        std::string getRealName() const;
};

#endif