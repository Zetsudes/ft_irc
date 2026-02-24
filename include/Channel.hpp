/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:14 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/24 14:55:09 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <set>

class Client; 
class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::set<Client*> _members;
        std::set<Client*> _operators;

        bool                _inviteOnly;       // +i
        std::set<Client*>   _invitedClients;   // +i
        bool                _topicRestricted;  // +t
        std::string         _key;              // +k
        size_t              _userLimit;        // +l

    public:
        Channel(const std::string& name);
        ~Channel();

        std::string getName() const;    
        std::string getTopic() const;                
        const std::set<Client*>& getMembers() const;    
        const std::set<Client*>& getOperators() const;

        void setTopic(const std::string& topic);

        void addClient(Client* client);
        void removeClient(Client* client);
        bool isMember(Client* client) const;

        void addOperator(Client* client);
        void removeOperator(Client* client);
        bool isOperator(Client* client) const;

        size_t memberCount() const;
        size_t operatorCount() const;
        void clearChannel();

        void setInviteOnly(bool status);
        bool isInviteOnly() const;
        void inviteClient(Client* client);
        bool isInvited(Client* client) const;
        void removeInvite(Client* client);

        void setTopicRestricted(bool status);
        bool isTopicRestricted() const;

        void setKey(const std::string& key);
        std::string getKey() const;
        bool hasKey() const;

        void setUserLimit(size_t limit);
        size_t getUserLimit() const;
        bool isFull() const;
};

#endif