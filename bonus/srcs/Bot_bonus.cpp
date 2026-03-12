/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot_bonus.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:26:07 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/03/12 16:38:20 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Bot_bonus.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

Bot::Bot() : nickname("Bot-anic"), username("Bot-terfly"), realname("Bot-oxx") {}

Bot::~Bot() {}

static bool containsCommand(const std::string &commandText, const std::string &command) {
	return commandText.find(command) != std::string::npos;
}

std::string Bot::respond(const std::string &receivedMessage, const std::string &from)
{
	if(containsCommand(receivedMessage, "hug"))
		return "PRIVMSG " + from + " :*virtual hug* 🤗";
	if(containsCommand(receivedMessage, "joke"))
		return "PRIVMSG " + from + " :Why are programmers so skinny? Because they refuse cookies. 🍪";
	return "";
}
void Bot::setFd(int fd) { this->botFd = fd; }

int Bot::getFd() const { return botFd; }

void Bot::connectToServer(const std::string& ip, int port, const std::string& password)
{
	setFd(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	if (botFd < 0)
		throw std::runtime_error("Error: Failed to create bot socket");
	struct sockaddr_in server_address;
	std::memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	if (inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr) <= 0)
		throw std::runtime_error("Error: Invalid server IP");
	if (connect(botFd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
		throw std::runtime_error("Error: Failed to connect to server");
	if (fcntl(botFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error: Failed to set socket non-blocking");
}


void Bot::sendMessage(const std::string& msg)
{
	if (this->botFd < 0)
	throw std::runtime_error("Socket not connected");
    ssize_t n = write(botFd, msg.c_str(), msg.size());
    if (n < 0)
	throw std::runtime_error("Failed to send message");
}

void Bot::login() {
    sendMessage("NICK " + nickname + "\r\n");
    sendMessage("USER " + username + " 0 * :" + realname + "\r\n");
}

void Bot::joinChannel(const std::string& channel) {
	std::string joinCmd = "JOIN #" + channel + "\r\n";
	sendMessage(joinCmd);
}

void Bot::receiveMessage() 
{
    char buffer[512];
    ssize_t n = read(botFd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
		std::string receivedMessage(buffer);
        if (receivedMessage.find("PING") == 0) {
            std::string server = receivedMessage.substr(5);
            sendMessage("PONG " + server + "\r\n");
            return;
        }
        std::string from = "unknown";
        if (!receivedMessage.empty() && receivedMessage[0] == ':') {
            size_t spacePos = receivedMessage.find(' ');
            if (spacePos != std::string::npos)
                from = receivedMessage.substr(1, spacePos - 1);
        }
        std::string response = respond(receivedMessage, from);
        if (!response.empty()) {
            sendMessage(response + "\r\n");
        }
    } else if (n < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
        throw std::runtime_error("Error: Failed to read from socket");
    }
}

void Bot::run() {
	login();
	joinChannel("chatbot");
    while (true) {
        try {
            receiveMessage();
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}