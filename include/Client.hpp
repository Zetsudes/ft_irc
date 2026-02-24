/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:11 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/24 15:15:32 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <iostream>

class Client
{
	private:
		int			client_fd;
		std::string	serverIp;

	public:
		Client();
		Client(Client const &src);
		Client &operator=(Client const &other);
		~Client();

};

