/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:11 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/24 12:03:29 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <iostream>

class Client
{
	private:
		int	_client_fd;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _buffer;
		bool _registered;

	public:
		Client(int fd);
		~Client();

		int getFd() const;
		std::string& getBuffer();
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealname() const;

		void setNickname(const std::string& nick);
		void setUsername(const std::string& user);
		void setRealname(const std::string& real);

		void appendToBuffer(const std::string& data);
		void clearBuffer();
		void tryRegister();

		bool isRegistered() const;
};

