#include "Server.hpp"

void Server::createSocket()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_fd < 0)
		throw std::runtime_error("Failed to create server socket");

	int reuseOption = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseOption, sizeof(reuseOption));





	bind() → dire où est la porte
	port  6667

	listen() → mettre la porte en mode “prêt à accueillir du monde”

	accept() → laisser entrer un client

	fcntl(O_NONBLOCK) → ne pas bloquer si la porte est vide
}