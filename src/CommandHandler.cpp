/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:53:47 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/03/05 11:39:36 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CommandHandler.hpp"
#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Parsing.hpp"
#include "../include/Channel.hpp"

CommandHandler::CommandHandler(Server& server, Client& client) : _server(server), _client(client) {};
CommandHandler::~CommandHandler() {};

void	CommandHandler::handlePass(const Parsing& parsedCmd) {
	if (parsedCmd.params.empty())
	{
		std::string errorMsg = std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n";
		send(_client.getFd(), errorMsg .c_str(), errorMsg .size(), 0);
		return;
	}
	std::string pwd = parsedCmd.params[0];
	if (pwd == _server.getPassword()) {
		_client.tryRegister();
	} else {
		std::string errorMsg = std::string(ERR_PASSWDMISMATCH) + " :Password incorrect\r\n";
		send(_client.getFd(), errorMsg .c_str(), errorMsg.size(), 0);
	}
}

void	CommandHandler::handleNick(const Parsing& parsedCmd) {
	if (parsedCmd.params.empty())
	{
		std::string errorMsg  = std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n";
		send(_client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	std::string nickname = parsedCmd.params[0];
	Client* other = _server.getClientByNickname(nickname);
   	if (other && other != &_client) {
		std::string errorMsg = std::string(ERR_NICKNAMEINUSE) + " :Nickname is already in use\r\n";
		send(_client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
		return;
    }
	_client.setNickname(nickname);
	_client.tryRegister();
}

void	CommandHandler::handleUser(const Parsing& parsedCmd) {
	(void)_server;
	if (parsedCmd.params.size() < 4) {
		std::string errorMsg = std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters\r\n";
		send(_client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	std::string username = parsedCmd.params[0];
	std::string hostname = parsedCmd.params[1];
	std::string servername = parsedCmd.params[2];
    std::string realname = parsedCmd.params[3];
    _client.setUsername(username);
    _client.setRealname(realname);
	_client.tryRegister();
}

void CommandHandler::handlePrivmsg(const Parsing& parsedCmd) {
	if (parsedCmd.params.size() < 2) {
		std::string errorMsg = std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters\r\n";
		send(_client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	std::string userDest = parsedCmd.params[0];
	std::string msg = parsedCmd.params[1];
	if (!_server.getClientByNickname(userDest)) {
		std::string errorMsg = std::string(ERR_NOSUCHNICK) + " " + userDest + " :No such nick/channel\r\n";
		send(_client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	std::string msgToSend = ":" + _client.getNickname() + " PRIVMSG " + userDest + " :" + msg + "\r\n";
	send(_server.getClientByNickname(userDest)->getFd(), msgToSend.c_str(), msgToSend.size(), 0);
}

/*void	CommandHandler::handleJoin(const Parsing& parsedCmd) {

}

void	CommandHandler::handlePart(const Parsing& parsedCmd) {

}

void	CommandHandler::handleTopic(const Parsing& parsedCmd) {

}

void	CommandHandler::handleKick(const Parsing& parsedCmd) {

}*/

void	CommandHandler::handleQuit(const Parsing& parsedCmd) {
	std::string reason;
	if (parsedCmd.params.empty()) {
		reason = "Client quit";
	} else {
		reason = parsedCmd.params[0];
	}
	_server.announceQuit(_client, reason);
	_server.removeClient(_client.getFd());
}
