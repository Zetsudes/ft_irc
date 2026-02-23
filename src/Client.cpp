/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:28:00 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/23 13:46:34 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// constructeur etc..


void Client::connectToServer()
{
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0)
		throw std::runtime_error("Error: Failed to create server socket");
	


}
