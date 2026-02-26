#pragma once

#include <string>
#include <vector>

class Parsing {
	public:
		std::string author;
		std::string request;
		std::vector<std::string> params;

		Parsing() {}
		~Parsing() {}

		static Parsing parse(const std::string& line);
};
