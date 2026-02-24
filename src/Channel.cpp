/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:12:50 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/24 14:45:37 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/Channel.hpp"
# include "../include/Client.hpp"

Channel::Channel(const std::string& name)
    : _name(name),
      _inviteOnly(false),
      _topicRestricted(false),
      _userLimit(0)
{        
}

Channel::~Channel() {}


// getters
std::string Channel::getName() const { return _name; }
std::string Channel::getTopic() const { return _topic; }
const std::set<Client*>& Channel::getMembers() const { return _members; }
const std::set<Client*>& Channel::getOperators() const { return _operators; }


void Channel::setTopic(const std::string& topic) { _topic = topic; }

// clients
void Channel::addClient(Client* client) {
    _members.insert(client);
}

void Channel::removeClient(Client* client) {
    _members.erase(client);
    _operators.erase(client);
    _invitedClients.erase(client);
}

bool Channel::isMember(Client* client) const {
    return _members.count(client);
}

size_t Channel::memberCount() const {
    return _members.size();
}


// operators
void Channel::addOperator(Client* client) {
    if (isMember(client))
        _operators.insert(client);
}

void Channel::removeOperator(Client* client) {
    _operators.erase(client);
}
bool Channel::isOperator(Client* client) const {
    return _operators.count(client);
}

size_t Channel::operatorCount() const {
    return _operators.size(); }

    
// +i
void Channel::setInviteOnly(bool status) {
    _inviteOnly = status;
}

bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

void Channel::inviteClient(Client* client) {
    _invitedClients.insert(client);
}

bool Channel::isInvited(Client* client) const {
    return _invitedClients.count(client);
}

void Channel::removeInvite(Client* client) {
    _invitedClients.erase(client);
}


// +t
void Channel::setTopicRestricted(bool status) {
    _topicRestricted = status;
}

bool Channel::isTopicRestricted() const {
    return _topicRestricted;
}


// +k
void Channel::setKey(const std::string& key) {
    _key = key;
}

std::string Channel::getKey() const {
    return _key;
}

bool Channel::hasKey() const {
    return !_key.empty();
}


// +l
void Channel::setUserLimit(size_t limit) {
    _userLimit = limit;
}

size_t Channel::getUserLimit() const {
    return _userLimit;
}

bool Channel::isFull() const {
    return (_userLimit != 0 && _members.size() >= _userLimit);
}