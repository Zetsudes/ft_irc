#pragma once

#include <string>
#include <vector>

class Command {
	public:
		std::string author;
		std::string request;
		std::vector<std::string> params;

		Command() {}
		~Command() {}

		static Command parse(const std::string& line);
};
