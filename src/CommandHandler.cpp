/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:53:47 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/26 13:05:16 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {}
CommandHandler::~CommandHandler() {};

void	CommandHandler::handlePass(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	if (parsedCmd.params.empty())
	{
		std::string msg = "Error :Password incorrect\r\n";
		send(clientInfo.getFd(), msg.c_str(), msg.size(), 0);
		return;
	}
	std::string pwd = parsedCmd.params[0];
    if (pwd == serverInfo.getPassword()) {
        clientInfo.tryRegister();  
    } else {
        std::string msg = "Error :Password incorrect\r\n";
        send(clientInfo.getFd(), msg.c_str(), msg.size(), 0);
    }
}
	
void	CommandHandler::handleNick(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
}

void	CommandHandler::handleUser(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd) {
	
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

