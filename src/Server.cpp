#include "Server.hpp"

void Server::createSocket()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_fd < 0)
		throw std::runtime_error("Error: Failed to create server socket");

	int reuseOptionOn = 1;
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseOptionOn, sizeof(reuseOption)) == -1)
		throw std::runtime_error("Failed to set socket options");

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	adress.sin_port = htons(6667);
	adress.sin_addr.s_addr = INADDR_ANY;

	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("Error: Failed to bind server socket");

	if(listen(server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: Could not listen");
	
	if(fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error: Failed to set socket options");
}

void Server::serverLoop()
{
	accept()
}
