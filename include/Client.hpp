/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:11 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/23 15:51:29 by pmeimoun         ###   ########.fr       */
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

		void	connectToServer();
		void	authenticate();
		void	clientLoop();
};

