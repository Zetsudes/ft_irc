/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:25:37 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/26 10:16:28 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Client.hpp"
#include "include/Channel.hpp"
#include "include/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    try
    {
        Server server(port, password);
        server.serverLoop(); 
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
