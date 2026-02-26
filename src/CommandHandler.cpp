/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:53:47 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/26 14:12:45 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {}
CommandHandler::~CommandHandler() {};

void	CommandHandler::handlePass(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	if (parsedCmd.params.empty())
	{
		std::string msg = std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n";
		send(clientInfo.getFd(), msg.c_str(), msg.size(), 0);
		return;
	}
	std::string pwd = parsedCmd.params[0];
	if (pwd == serverInfo.getPassword()) {
		clientInfo.tryRegister();  
	} else {
		std::string msg = ERR_PASSWDMISMATCH;
		send(clientInfo.getFd(), msg.c_str(), msg.size(), 0);
	}
}
	
void	CommandHandler::handleNick(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	if (parsedCmd.params.empty())
	{
		std::string msg =ERR_NONICKNAMEGIVEN;
		send(clientInfo.getFd(), msg.c_str(), msg.size(), 0);
		return;
	}
	std::string nickname = parsedCmd.params[0];
	Client* other = serverInfo.getClientByNickname(nickname);
   	if (other && other != &clientInfo) {
		std::string msg = ERR_NICKNAMEINUSE;
		send(clientInfo.getFd(), msg.c_str(), msg.size(), 0);
		return;
    }
	clientInfo.setNickname(nickname);
	clientInfo.tryRegister();
}

void	CommandHandler::handleUser(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	if (parsedCmd.params.size() < 4) {
		std::string msg = ERR_NEEDMOREPARAMS; 
		send(clientInfo.getFd(), msg.c_str(), msg.size(), 0);
		return;
	}
	std::string username = parsedCmd.params[0];
	std::string hostname = parsedCmd.params[1];
	std::string servername = parsedCmd.params[2];
    std::string realname = parsedCmd.params[3];
    clientInfo.setUsername(username);
    clientInfo.setRealname(realname);
	clientInfo.tryRegister();
}

void	CommandHandler::handlePrivmsg(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
}

void	CommandHandler::handleJoin(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
}

void	CommandHandler::handlePart(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
}

void	CommandHandler::handleTopic(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
}

void	CommandHandler::handleKick(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
}

void	CommandHandler::handleQuit(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
}

