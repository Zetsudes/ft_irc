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
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (_client.isRegistered())
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_ALREADYREGISTERED) + " :You may not reregister (≖_≖ )\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string pwd = parsedCmd.params[0];
	if (pwd != _server.getPassword()) 
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_PASSWDMISMATCH) + " :Password incorrect 凸( •̀_•́ )凸\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
	}
	else
   		_client.setPassAccepted(true);
}

void	CommandHandler::handleNick(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.empty())
	{
		std::string errorMsg  = ":ircserv " + std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given (╭ರ_•́)\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (_client.isRegistered())
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_ALREADYREGISTERED) + " :You may not reregister (≖_≖ )\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string nickname = parsedCmd.params[0];
	Client* other = _server.getClientByNickname(nickname);
   	if (other && other != &_client) 
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NICKNAMEINUSE) + std::string(_client.getNickname()) + " :Nickname is already in use ( ≖‿  ≖ )Heehee\r\n";
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
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
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
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string userDest = parsedCmd.params[0];
	std::string msg = parsedCmd.params[1];

	Client* target = _server.getClientByNickname(userDest);
	if (!target) {
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHNICK) + " " + userDest + " :No such nick/channel ¯\\_(ツ)_/¯\r\n";
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
				std::string errorMsg = ":ircserv " + std::string(ERR_INVITEONLYCHAN) + name + " :Cannot join channel (+i) ⎛⎝( ` ᢍ ´ )⎠⎞ᵐᵘʰᵃʰᵃ\r\n";
				_client.appendToBuffer(errorMsg);
				_server.handlePollout(_client);
				return;
			}
			if (channel->isFull())
			{
				std::string errorMsg = ":ircserv " + std::string(ERR_CHANNELISFULL) + name + " :Cannot join channel (+l ⎛⎝( ` ᢍ ´ )⎠⎞ᵐᵘʰᵃʰᵃ)\r\n";
				_client.appendToBuffer(errorMsg);
				_server.handlePollout(_client);
				return;
			}
			if (channel->hasKey())
			{
				if (key.empty() || key != channel->getKey())
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_BADCHANNELKEY) + name + " :Cannot join channel (+k) ⎛⎝( ` ᢍ ´ )⎠⎞ᵐᵘʰᵃʰᵃ\r\n";
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
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
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

void	CommandHandler::handleMode(const Parsing& parsedCmd) 
{
	std::string name = parsedCmd.params[0];
	Channel* channel = _server.getChannel(name);
	if (parsedCmd.params.size() == 1)
	{
		if (!channel)
		{
			std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHCHANNEL) + " " + name + " :No such channel ¯\\_(ツ)_/¯\r\n";
			_client.appendToBuffer(errorMsg);
			_server.handlePollout(_client);
			return;
		}
		std::string modes = "+";
		std::string modeParams;
		if (channel->isInviteOnly())
			modes += 'i';
		if (channel->isTopicRestricted())
			modes += 't';
		if (channel->hasKey())
		{
			modes += 'k';
			modeParams += " " + channel->getKey();
		}
		if (channel->getUserLimit() != 0)
		{
			modes += 'l';
			std::ostringstream oss;
			oss << channel->getUserLimit();
			modeParams += " " + oss.str();
		}
		std::string rpl = ":ircserv " + std::string(RPL_CHANNELMODEIS) + " " + _client.getNickname() + " " + name + " " + modes + modeParams + "\r\n";
		_client.appendToBuffer(rpl);
		_server.handlePollout(_client);
		return;
	}
	if (parsedCmd.params.size() < 2)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (!channel)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHCHANNEL) + " " + name + " :No such channel ¯\\_(ツ)_/¯\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (!(channel->isMember(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOTONCHANNEL) + " :You are not on that channel ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (!(channel->isOperator(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_CHANOPRIVSNEEDED) + " :You are not channel operator ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}

	std::string mode = parsedCmd.params[1];
	size_t j = 2;
	bool plus;

	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '+')
			plus = true;
		else if (mode[i] == '-')
			plus = false;
		else if (mode[i] == 'i')
		{
			if (plus)
				channel->setInviteOnly(true);
			else 
				channel->setInviteOnly(false);
		}
		else if (mode[i] == 't')
		{
			if (plus)
				channel->setTopicRestricted(true);
			else
				channel->setTopicRestricted(false);
		}
		else if (mode[i] == 'k')
		{
			if (plus)
			{
				if (j >= parsedCmd.params.size())
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client);
					return;
				}
				channel->setKey(parsedCmd.params[j]);
				j++;
			}
			else
				channel->setKey("");
		}
		else if (mode[i] == 'l')
		{
			if (plus)
			{
				if (j >= parsedCmd.params.size())
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client);
					return;
				}
				channel->setUserLimit(std::atoi(parsedCmd.params[j].c_str()));
				j++;
			}
			else
				channel->setUserLimit(0);
		}
		else if (mode[i] == 'o')
		{
			if (plus)
			{
				if (j >= parsedCmd.params.size())
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client);
					return;
				}
				Client* target = _server.getClientByNickname(parsedCmd.params[j]);
				if (!target)
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHNICK) + " " + parsedCmd.params[0] + " :No such nick ¯\\_(ツ)_/¯\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client); 
					return;
				}
				if (!(channel->isMember(target)))
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_USERNOTINCHANNEL) + " " + target->getNickname() + " " + name + " :They are not on that channel ( ＾◡＾)っ NO\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client);
					return;
				}
				if (channel->isOperator(target))
					return;
				channel->addOperator(target);
				j++;
			}
			else
			{
				if (j >= parsedCmd.params.size())
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client);
					return;
				}
				Client* target = _server.getClientByNickname(parsedCmd.params[j]);
				if (!target)
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHNICK) + " " + parsedCmd.params[0] + " :No such nick ¯\\_(ツ)_/¯\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client); 
					return;
				}
				if (!(channel->isMember(target)))
				{
					std::string errorMsg = ":ircserv " + std::string(ERR_USERNOTINCHANNEL) + " " + target->getNickname() + " " + name + " :They are not on that channel ( ＾◡＾)っ NO\r\n";
					_client.appendToBuffer(errorMsg);
					_server.handlePollout(_client);
					return;
				}
				channel->removeOperator(target);
				j++;
			}
		}
		else 
		{
			std::string errorMsg = ":ircserv " + std::string(ERR_UNKNOWNMODE) + " " + std::string(1, mode[i]) + " :is unknown mode char to me ∘ ∘ ∘ ( °ヮ° ) ?\r\n";
			_client.appendToBuffer(errorMsg);
			_server.handlePollout(_client);
			return;
		}
	}
	std::string broadcastMsg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost MODE " + name + " " + parsedCmd.params[1];
	for (size_t k = 2; k < parsedCmd.params.size(); k++)
		broadcastMsg += " " + parsedCmd.params[k];
	broadcastMsg += "\r\n";
	const std::set<Client*>& members = channel->getMembers();
	for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		(*it)->appendToBuffer(broadcastMsg);
		_server.handlePollout(**it);
	}
}

void	CommandHandler::handleInvite(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.size() <= 1)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	Client* target = _server.getClientByNickname(parsedCmd.params[0]);
	std::string name = parsedCmd.params[1];
	Channel* channel = _server.getChannel(name);
	if (!target)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHNICK) + " " + parsedCmd.params[0] + " :No such nick ¯\\_(ツ)_/¯\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (!channel)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHCHANNEL) + " " + name + " :No such channel ¯\\_(ツ)_/¯\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (!(channel->isMember(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOTONCHANNEL) + " :You are not on that channel ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (channel->isInviteOnly() && !(channel->isOperator(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_CHANOPRIVSNEEDED) + " :You are not channel operator ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (channel->isMember(target))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_USERONCHANNEL) + target->getUsername() + " " + name + " :is already on channel (¬_¬)\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	std::string clientMsg = ":ircserv " + std::string(RPL_INVITING) + " " + _client.getNickname() + " " +  name + " " + target->getNickname() + "\r\n";
	_client.appendToBuffer(clientMsg);
	_server.handlePollout(_client); 

	std::string inviteMsg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost INVITE " + target->getNickname() + " " + name + "\r\n";
	target->appendToBuffer(inviteMsg);
	_server.handlePollout(*target); 

	channel->inviteClient(target);
}

void	CommandHandler::partChannel(const std::string& name, const std::string& reason)
{
	Channel* channel = _server.getChannel(name);
	if (!channel)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHCHANNEL) + name + " :No such channel ¯\\_(ツ)_/¯\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (!(channel->isMember(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOTONCHANNEL) + name + " :You are not on that channel ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}

	std::string partMsg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost PART " + name + " :" + reason + "\r\n";
	const std::set<Client*>& members = channel->getMembers();
	for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		(*it)->appendToBuffer(partMsg);
		_server.handlePollout(**it);
	}

	channel->removeClient(&_client);

	if (channel->memberCount() == 0)
    _server.removeChannel(name);
}
void	CommandHandler::handlePart(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.size() < 1)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}

	std::string reason;
	if (parsedCmd.params.size() >= 2)
		reason = parsedCmd.params[1];
	else
		reason = _client.getNickname();

	std::string list = parsedCmd.params[0];
	size_t start = 0;
	size_t pos;

	while ((pos = list.find(",", start)) != std::string::npos)
	{
		std::string name = list.substr(start, pos - start);
		partChannel(name, reason);
		start = pos + 1;
	}
	std::string name = list.substr(start);
	partChannel(name, reason);
}

void	CommandHandler::handleTopic(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.size() < 1)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string name = parsedCmd.params[0];
	Channel* channel = _server.getChannel(name);
	if (!channel)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHCHANNEL) + " :No such channel ¯\\_(ツ)_/¯\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (!(channel->isMember(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOTONCHANNEL) + " :You are not on that channel ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (channel->isTopicRestricted() && !(channel->isOperator(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_CHANOPRIVSNEEDED) + " :You are not channel operator ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (parsedCmd.params.size() == 1)
	{
		if (!(channel->getTopic().empty()))
		{
			std::string topicMsg = ":ircserv " + std::string(RPL_TOPIC) + name + " :" + channel->getTopic() + "\r\n";
			_client.appendToBuffer(topicMsg);
			_server.handlePollout(_client);
			return;
		}
		else
		{
			std::string noTopicMsg = ":ircserv " + std::string(RPL_NOTOPIC) + name + " :No topic is set ⓘ\r\n";
			_client.appendToBuffer(noTopicMsg);
			_server.handlePollout(_client); 
			return;
		}
	}
	std::string topic = parsedCmd.params[1];
	channel->setTopic(topic);

	std::string broadcastMsg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost TOPIC " + name + " :" + topic + "\r\n";
	const std::set<Client*>& members = channel->getMembers();
	for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		(*it)->appendToBuffer(broadcastMsg);
		_server.handlePollout(**it);
	}
}

void	CommandHandler::handleKick(const Parsing& parsedCmd) 
{
	if (parsedCmd.params.size() <= 1)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " :Not enough parameters <(ꐦㅍ _ㅍ)>\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	std::string name = parsedCmd.params[0];
	Channel* channel = _server.getChannel(name);
	if (!channel)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHCHANNEL) + " :No such channel ¯\\_(ツ)_/¯\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client);
		return;
	}
	if (!(channel->isMember(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOTONCHANNEL) + " :You are not on that channel ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (!(channel->isOperator(&_client)))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_CHANOPRIVSNEEDED) + " :You are not channel operator ( ＾◡＾)っ NO\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	Client* target = _server.getClientByNickname(parsedCmd.params[1]);
	if (!target)
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_NOSUCHNICK) + " " + parsedCmd.params[1] + " :No such nick ¯\\_(ツ)_/¯\r\n";
		_client.appendToBuffer(errorMsg);
		_server.handlePollout(_client); 
		return;
	}
	if (!channel->isMember(target))
	{
		std::string errorMsg = ":ircserv " + std::string(ERR_USERNOTINCHANNEL) + " " + target->getNickname() + " " + name + " :They are not on that channel ( ＾◡＾)っ NO\r\n";
    	_client.appendToBuffer(errorMsg);
    	_server.handlePollout(_client);
    	return;
	}
	std::string reason;
	if (parsedCmd.params.size() > 2)
		reason = parsedCmd.params[2];
	else
		reason = _client.getNickname();

	std::string broadcastMsg = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost KICK " + name + " " + target->getNickname() + " :" + reason + "\r\n";
	const std::set<Client*>& members = channel->getMembers();
	for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		(*it)->appendToBuffer(broadcastMsg);
		_server.handlePollout(**it);
	}

	channel->removeClient(target);
}

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
