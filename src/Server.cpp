#include "Server.hpp"

void Server::createSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_fd < 0)
		throw std::runtime_error("Error: Failed to create server socket");

	int reuseOptionOn = 1;
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseOptionOn, sizeof(reuseOptionOn)) == -1)
		throw std::runtime_error("Error: Failed to set socket options");

	struct sockaddr_in server_address = {0};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(6667);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if(bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
		throw std::runtime_error("Error: Failed to bind server socket");

	if(listen(server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: Could not listen");
	
	if(fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error: Failed to set socket options");
}

void Server::serverLoop()
{
	while(true)
	{
		struct sockaddr_in client_address;
		socklen_t client_len = sizeof(client_address);
		int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_len);
		if (client_fd < 0)	
		{
			if(errno == EAGAIN) 
			continue;
		}
		else
		{
			std::cerr << "Error: Failed to accept client, server stopped." << std::endl;
			break;
		}
		
			
		

	}


}
