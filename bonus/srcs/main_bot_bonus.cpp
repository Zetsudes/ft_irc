#include "../include/Bot_bonus.hpp"
#include <iostream>

int main()
{
	Bot NewBot;
	try {
		NewBot.connectToServer("127.0.0.1", 6667, "pwd");
		NewBot.run();
	}
	catch (const std::exception &e) {
		std::cerr << "Bot error: " << e.what() << std::endl;
	}
	return 0;
}