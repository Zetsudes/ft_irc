/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:11 by zamohame          #+#    #+#             */
/*   Updated: 2026/03/09 09:33:36 by pmeimoun         ###   ########.fr       */
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
		bool _passAccepted;

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
		void setPassAccepted(bool accepted);
};

