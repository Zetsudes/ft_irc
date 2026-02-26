/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:54:05 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/26 14:41:05 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "Parsing.hpp"

//Authentification errors
#define ERR_NONICKNAMEGIVEN		"431"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_NEEDMOREPARAMS		"461"
#define ERR_PASSWDMISMATCH		"464"
#define ERR_NOSUCHNICK			"401"

// Channel errors
#define ERR_NOSUCHCHANNEL   "403"    // No such channel
#define ERR_CHANOPRIVSNEEDED "482"  // You're not channel operator

class CommandHandler
{
	public:
		CommandHandler() {};
		~CommandHandler() {};
		
		//Authentification
		void	handlePass(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); //indiquer le mot de passe
		void	handleNick(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); //permet de changer de nickname
		void	handleUser(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd);
		
		//Messages
		void	handlePrivmsg(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); //permet d'envoyer un message en privé à un utilisateur
		
		//Channels
		void	handleJoin(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); //permet d'aller sur un channel
		void	handlePart(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); //permet de partir d'un ou de plusieurs channels
		void	handleTopic(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); //permet de changer le topic du channel (titre d'un maximum de 80 caractères)
		void	handleKick(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); //permet de virer quelqu'un du channel
		
		//Déconnexion
		void	handleQuit(Server& serverInfo, Client& clientInfo, const Parsing& parsedCmd); // permez de quitter IRC
};