/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:11 by zamohame          #+#    #+#             */
/*   Updated: 2026/03/09 11:57:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <iostream>

class Server;

class Client
{
	private:
		int	_client_fd;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _buffer;
		std::string _parseBuffer;
		bool _registered;
		bool _passAccepted;

	public:
		Client(int fd);
		~Client();

		int getFd() const;
		std::string& getBuffer();
		std::string& getParseBuffer();
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealname() const;

		void setNickname(const std::string& nick);
		void setUsername(const std::string& user);
		void setRealname(const std::string& real);
		void setPassAccepted(bool accepted);

		void appendToBuffer(const std::string& data);
		void appendToParseBuffer(const std::string& data);
		void clearBuffer();
		bool tryRegister();

		bool isRegistered() const;
};

