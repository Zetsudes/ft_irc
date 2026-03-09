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
#include <string>

//Authentification errors
#define ERR_NONICKNAMEGIVEN		"431"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_NEEDMOREPARAMS		"461"
#define ERR_PASSWDMISMATCH		"464"
#define ERR_NOSUCHNICK			"401"
#define ERR_ALREADYREGISTERED   "462"

// Channel errors
#define ERR_NOSUCHCHANNEL   "403"    // No such channel
#define ERR_CHANOPRIVSNEEDED "482"  // You're not channel operator
#define ERR_BADCHANNELKEY       "475"  // wrong channel password (+k)
#define ERR_INVITEONLYCHAN      "473"  // channel is +i and user not invited
#define ERR_CHANNELISFULL       "471"  // channel is +l and full
#define ERR_NOTONCHANNEL        "442"  // client not on channel
#define ERR_USERONCHANNEL       "443"  // INVITE but user already on channel
#define ERR_USERNOTINCHANNEL    "441"  // KICK target not on channel

// Replies
#define RPL_WELCOME             "001"
#define RPL_YOURHOST            "002"
#define RPL_CREATED             "003"
#define RPL_MYINFO              "004"
#define RPL_INVITING            "341"  // successful INVITE
#define RPL_NOTOPIC             "331"  // channel has no topic
#define RPL_TOPIC               "332"  // channel topic
#define RPL_NAMREPLY            "353"  // list of users in channel after JOIN
#define RPL_ENDOFNAMES          "366"  // end of NAMES list

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
		void 	joinChannel(const std::string& name, const std::string& key); //helper
		void	handlePart(const Parsing& parsedCmd); //permet de partir d'un ou de plusieurs channels
		void	partChannel(const std::string& name, const std::string& reason); //helper
		void	handleTopic(const Parsing& parsedCmd); //permet de changer le topic du channel (titre d'un maximum de 80 caractères)
		void	handleKick(const Parsing& parsedCmd); //permet de virer quelqu'un du channel
		void	handleInvite(const Parsing& parsedCmd); //permet de gérer les invitations
		void	handleMode(const Parsing& parsedCmd); //permet de gérer les modes du channel

		//Déconnexion
		void	handleQuit(const Parsing& parsedCmd); // permez de quitter IRC

};
