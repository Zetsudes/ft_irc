/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:54:05 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/26 11:16:02 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "Parsing.hpp"

class CommandHandler
{
	public:
		CommandHandler() {};
		~CommandHandler() {};
		
		//Authentification
		void	handlePass(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		void	handleNick(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		void	handleUser(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		
		//Messages
		void	handlePrivmsg(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		
		//Channels
		void	handleJoin(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		void	handlePart(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		void	handleTopic(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		void	handleKick(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		
		//Déconnexion
		void	handleQuit(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
};