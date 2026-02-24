/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:25:37 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/24 14:16:29 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Client.hpp"
#include "include/Channel.hpp"
#include "include/Server.hpp"

int main()
{
	//int port = std::atoi(av[1]);
	std::cout << " <3 Client Class <3" << std::endl << std::endl;
	Client c(42);
    std::cout << "Registered? " << c.isRegistered() << std::endl;

    c.setNickname("le z");
    std::cout << "Registered after nick: " << c.isRegistered() << std::endl;

    c.setUsername("kangoojuniordu06");
    std::cout << "Registered after username: " << c.isRegistered() << std::endl;

    c.setRealname("zai");
    std::cout << "Buffer before append: " << c.getBuffer() << std::endl;

    c.appendToBuffer("Rendez-vous tous a school island on vous attend sur l'ile du bonheeuuuuurrrrrrr");
    std::cout << "Buffer after append: " << c.getBuffer() << std::endl;

	std::cout << "Fd : " <<  c.getFd() << std::endl;
	std::cout << "Realname: " << c.getRealname() << std::endl;
	std::cout << "Nickname: " << c.getNickname() << std::endl;
	std::cout << "Username: " << c.getUsername() << std::endl;

    c.clearBuffer();
    std::cout << "Buffer after clear: " << c.getBuffer() << std::endl << std::endl;

	std::cout << " <3 Channel Class <3" << std::endl << std::endl;
	Channel chanchan("mychannel");
	Client Corneil(1), Bernie(2);

	chanchan.addClient(&Corneil);
	chanchan.addClient(&Bernie);
	chanchan.addOperator(&Corneil);

	std::cout << "Members: " << chanchan.memberCount() << std::endl;  
	std::cout << "Operators: " << chanchan.operatorCount() << std::endl;

	chanchan.removeOperator(&Corneil);
	std::cout << "Operators after Corneil died :( : " << chanchan.operatorCount() << std::endl;

    return 0;
}
