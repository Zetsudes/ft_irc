/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:28:00 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/26 12:57:42 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd) 
	: _client_fd(fd),
	  _registered(false)
{
}
Client::~Client() {}


// getters
int Client::getFd () const { return _client_fd; }
std::string& Client::getBuffer() { return _buffer; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getRealname() const { return _realname; }


//setters
void Client::setNickname(const std::string& nick) {
	_nickname = nick;
	tryRegister();
}

void Client::setUsername(const std::string& user) {
	_username = user;
	tryRegister();
}

void Client::setRealname(const std::string& real) { _realname = real; }


void Client::appendToBuffer(const std::string& data) { _buffer += data; }
void Client::clearBuffer() { _buffer.clear(); }
void Client::tryRegister() {
	if(!_nickname.empty() && !_username.empty())
		_registered = true;
}

bool Client::isRegistered() const { return _registered; }
