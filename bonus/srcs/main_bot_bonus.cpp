#include "../include/Bot_bonus.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 3)
    {
        std::cerr << "Usage: ./bot_bonus <port> <password>" << std::endl;
        return 1;
    }

	Bot NewBot;
	try {
		int port = std::atoi(argv[1]);
		std::string pass = argv[2];
		NewBot.connectToServer("127.0.0.1", port, pass); 
		NewBot.run();
	}
	catch (const std::exception &e) {
		std::cerr << "Bot error: " << e.what() << std::endl;
	}
	return 0;
}