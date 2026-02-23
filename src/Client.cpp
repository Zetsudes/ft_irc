/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:28:00 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/23 15:51:02 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// constructeur etc..


void Client::connectToServer()
{
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0)
		throw std::runtime_error("Error: Failed to create server socket");
	struct sockaddr_in server_address = {0};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(6667);
	serverIp = "127.0.0.1";
	server_address.sin_addr.s_addr = inet_addr(serverIp.c_str());
	if(server_address.sin_addr.s_addr == INADDR_NONE)
		throw std::runtime_error("Error: Invalid IP address");
	if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
		throw std::runtime_error("Error: Connection failed");
}

void Client::authenticate()
{

}

void	Client::clientLoop()
{
	
}
