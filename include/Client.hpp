/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:11 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/23 15:51:29 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <iostream>

class Client
{
	private:
		int	client_fd;
		std::string nickname;
		std::string username;
		std::string realname;
		std::string buffer;

		bool hasNick;
		bool hasUser;
		bool registered;
		// std::string	serverIp;

	public:
		Client();
		Client(int fd);
		Client(const Client& src);
		Client &operator=(const Client& other);
		~Client();

		int getFd() const;

		void setNickname(const std::string& nick);
		void setUsername(const std::string& user);
		void setRealname(const std::string& real);

		std::string getNickName() const;

		void appendToBuffer(const std::string& data);
		void clearBuffer();

		bool isRegistered() const;
		std::string& getBuffer();
};

