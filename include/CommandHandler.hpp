/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:54:05 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/03/05 11:40:08 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//Authentification errors
#define ERR_NONICKNAMEGIVEN		"431"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_NEEDMOREPARAMS		"461"
#define ERR_PASSWDMISMATCH		"464"
#define ERR_NOSUCHNICK			"401"

// Channel errors
#define ERR_NOSUCHCHANNEL   "403"    // No such channel
#define ERR_CHANOPRIVSNEEDED "482"  // You're not channel operator

class Server;
class Client;
class Parsing;

class CommandHandler
{
	private: 
		Server& _server;
		Client& _client;

	public:
		CommandHandler(Server& server, Client& client);
		~CommandHandler();

		//Authentification
		void	handlePass(const Parsing& parsedCmd); //indiquer le mot de passe
		void	handleNick(const Parsing& parsedCmd); //permet de changer de nickname
		void	handleUser(const Parsing& parsedCmd);

		//Messages
		void	handlePrivmsg(const Parsing& parsedCmd); //permet d'envoyer un message en privé à un utilisateur

		//Channels
		void	handleJoin(const Parsing& parsedCmd); //permet d'aller sur un channel
		void	handlePart(const Parsing& parsedCmd); //permet de partir d'un ou de plusieurs channels
		void	handleTopic(const Parsing& parsedCmd); //permet de changer le topic du channel (titre d'un maximum de 80 caractères)
		void	handleKick(const Parsing& parsedCmd); //permet de virer quelqu'un du channel

		//Déconnexion
		void	handleQuit(const Parsing& parsedCmd); // permez de quitter IRC

};
