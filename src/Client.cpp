/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:28:00 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/03/09 11:44:58 by marvin           ###   ########.fr       */
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
std::string& Client::getParseBuffer() { return _parseBuffer; }
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
void Client::appendToParseBuffer(const std::string& data) { _parseBuffer += data; }
void Client::clearBuffer() { _buffer.clear(); }
bool Client::tryRegister() {
	if(!_registered && !_nickname.empty())
		{
			_registered = true;
			return true;
		}
		return false;
}

bool Client::isRegistered() const { return _registered; }
