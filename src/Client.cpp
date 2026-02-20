/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:03:59 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/20 14:23:41 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


void Client::setNickName(const std::string &str) { nickName = str; }
void Client::setUserName(const std::string &str) { userName = str; }
void Client::setRealName(const std::string &str) { realName = str; }


std::string Client::getNickName() const { return nickName; }
std::string Client::getUserName() const { return userName; }
std::string Client::getRealName() const { return realName; }
