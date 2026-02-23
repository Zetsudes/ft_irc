/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:14 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/20 11:55:08 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>

class Client; 
class Channel
{
    private:
        std::string _name;
        std::set<Client*> _members;
        std::set<Client*> _operators;

    public:
        Channel();
        Channel(const std::string& name);
        Channel(const Channel& src);
        Channel &operator=(const Channel& other);
        ~Channel();

        void addClient(Client* client);
        void removeClient(Client* client);

        bool isMember(Client* client) const;
};

#endif