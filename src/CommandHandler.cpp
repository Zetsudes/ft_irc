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

void	CommandHandler::handlePass(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.empty())
	{
		std::string errorMsg = std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (_client.isRegistered())
	{
		std::string errorMsg = std::string(ERR_ALREADYREGISTERED) + " :You may not reregister\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string pwd = parsedCmd.params[0];
	if (pwd != _server.getPassword()) 
	{
		std::string errorMsg = std::string(ERR_PASSWDMISMATCH) + " :Password incorrect\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
	}
}

void	CommandHandler::handleNick(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.empty())
	{
		std::string errorMsg  = std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (_client.isRegistered())
	{
		std::string errorMsg = std::string(ERR_ALREADYREGISTERED) + " :You may not reregister\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string nickname = parsedCmd.params[0];
	Client* other = _server.getClientByNickname(nickname);
   	if (other && other != &_client) 
	{
		std::string errorMsg = std::string(ERR_NICKNAMEINUSE) + " :Nickname is already in use\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
    }
	_client.setNickname(nickname);
	if (_client.isRegistered())
		_server.sendWelcome(_client);
}

void	CommandHandler::handleUser(const Parsing& parsedCmd) 
{
	(void)_server;
	if (parsedCmd.params.size() < 4) {
		std::string errorMsg = std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string username = parsedCmd.params[0];
	std::string hostname = parsedCmd.params[1];
	std::string servername = parsedCmd.params[2];
    std::string realname = parsedCmd.params[3];
    _client.setUsername(username);
    _client.setRealname(realname);
}

void CommandHandler::handlePrivmsg(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.size() < 2) {
		std::string errorMsg = std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string userDest = parsedCmd.params[0];
	std::string msg = parsedCmd.params[1];

	Client* target = _server.getClientByNickname(userDest);
	if (!target) {
		std::string errorMsg = std::string(ERR_NOSUCHNICK) + " " + userDest + " :No such nick/channel\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string msgToSend = ":" + _client.getNickname() + " PRIVMSG " + userDest + " :" + msg + "\r\n";
	target->appendToBuffer(msgToSend);
	_server.handlePollout(*target);
}

void CommandHandler::joinChannel(const std::string& name, const std::string& key)
{
	Channel* channel = _server.getChannel(name);
	if (channel)
		{
			if (channel->isMember(&_client))
				return;
			if (channel->isInviteOnly() && !(channel->isInvited(&_client)))
			{
				std::string errorMsg = std::string(ERR_INVITEONLYCHAN) + std::string(name) + " :Cannot join channel (+i)\r\n";
				_client.appendToBuffer(errorMsg);
				_server.handlePollout(_client);
				return;
			}
			if (channel->isFull())
			{
				std::string errorMsg = std::string(ERR_CHANNELISFULL) + std::string(name) + " :Cannot join channel (+l)\r\n";
				_client.appendToBuffer(errorMsg);
				_server.handlePollout(_client);
				return;
			}
			if (channel->hasKey())
			{
				if (key.empty() || key != channel->getKey())
				{
					std::string errorMsg = std::string(ERR_BADCHANNELKEY) + std::string(name) + " :Cannot join channel (+k)\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client);
					return;
				}
			}
			channel->addClient(&_client);
			std::string joinMsg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost JOIN " + name + "\r\n";

			std::string rpl353 = ":ircserv 353 " + _client.getNickname() + " = " + name + " :";

			const std::set<Client*>& members = channel->getMembers();
			for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
			{
				if (channel->isOperator(*it))
					rpl353 += "@";
				rpl353 += (*it)->getNickname() + " ";
			}
			rpl353 += "\r\n";

			std::string rpl366 = ":ircserv 366 " + _client.getNickname() + " " + name + " :End of /NAMES list\r\n";

			_client.appendToBuffer(joinMsg);
			_client.appendToBuffer(rpl353);
			_client.appendToBuffer(rpl366);
			_server.handlePollout(_client);
		}
		else
		{
			channel = _server.createChannel(name);
			channel->addClient(&_client);
			channel->addOperator(&_client);
			std::string joinMsg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost JOIN " + name + "\r\n";

			std::string rpl353 = ":ircserv 353 " + _client.getNickname() + " = " + name + " :";

			const std::set<Client*>& members = channel->getMembers();
			for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
			{
				if (channel->isOperator(*it))
					rpl353 += "@";
				rpl353 += (*it)->getNickname() + " ";
			}
			rpl353 += "\r\n";

			std::string rpl366 = ":ircserv 366 " + _client.getNickname() + " " + name + " :End of /NAMES list\r\n";

			_client.appendToBuffer(joinMsg);
			_client.appendToBuffer(rpl353);
			_client.appendToBuffer(rpl366);
			_server.handlePollout(_client);
		}
}

void	CommandHandler::handleJoin(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.size() < 1)
	{
		std::string errorMsg = std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::vector<std::string> keys;
	if (parsedCmd.params.size() >= 2)
	{
		std::string keyList = parsedCmd.params[1];
		size_t kstart = 0;
		size_t kpos;
		while ((kpos = keyList.find(',', kstart)) != std::string::npos)
		{
			keys.push_back(keyList.substr(kstart, kpos - kstart));
			kstart = kpos + 1;
		}
		keys.push_back(keyList.substr(kstart)); 
	}
	std::string list = parsedCmd.params[0];
	size_t start = 0;
	size_t pos;
	size_t i = 0;
	while((pos = list.find(',', start)) != std::string::npos)
	{
		std::string name = list.substr(start, pos - start);
		std::string key;
		if (i < keys.size())
			key = keys[i];
		else
			key = "";
		joinChannel(name, key);
			i++;
		start = pos + 1;
	}
	std::string name = list.substr(start);
	std::string key;
	if (i < keys.size())
		key = keys[i];
	else
		key = "";
	joinChannel(name, key);
}

// void	CommandHandler::handleMode(const Parsing& parsedCmd) 
// {

// }

// void	CommandHandler::handleInvite(const Parsing& parsedCmd) 
// {

// }

// void	CommandHandler::handlePart(const Parsing& parsedCmd) 
// {

// }

// void	CommandHandler::handleTopic(const Parsing& parsedCmd) 
// {

// }

// void	CommandHandler::handleKick(const Parsing& parsedCmd) 
// {

// }

void	CommandHandler::handleQuit(const Parsing& parsedCmd) 
{
	std::string reason;
	if (parsedCmd.params.empty()) {
		reason = "Client quit";
	} else {
		reason = parsedCmd.params[0];
	}
	_server.announceQuit(_client, reason);
	_server.removeClient(_client.getFd());
}
