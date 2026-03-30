/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot_bonus.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:25:53 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/03/16 09:32:24 by pmeimoun         ###   ########.fr       */
=======
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:25:53 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/03/30 16:15:56 by zamohame         ###   ########.fr       */
>>>>>>> le-z
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

class Bot {
	private:
		int botFd;
		std::string nickname;
		std::string username;
		std::string realname;
	public:
		Bot();
		~Bot();
		void setFd(int);
		int getFd() const;
		std::string respond(const std::string &receivedMessage, const std::string &from);
		void connectToServer(const std::string &ip, int port, const std::string &password);
		void login();
		void joinChannel(const std::string& channel);
		void run();
		void sendMessage(const std::string& msg);
};
