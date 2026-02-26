/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:48:26 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/26 10:51:16 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parsing.hpp"

//constructeur

Parsing Parsing::parse(const std::string& line) {
	Parsing parse;
	size_t start = 0;
	size_t delimiterPos = 0;
	size_t len = line.length();

	if (!line.empty() && line[0] == ':') {
		delimiterPos = line.find(' ', start);
		if (delimiterPos != std::string::npos) {
			parse.author = line.substr(1, delimiterPos - 1);
			start = delimiterPos + 1;
		} else {
			parse.author = line.substr(1);
			return parse;
		}
	}
	delimiterPos = line.find(' ', start);
	if (delimiterPos != std::string::npos) {
		parse.request = line.substr(start, delimiterPos - start);
		start = delimiterPos + 1;
	} else {
		parse.request = line.substr(start);
		return parse;
	}
	while (start < len) {
		if (line[start] == ':') {
			parse.params.push_back(line.substr(start + 1));
			break;
		}
		delimiterPos = line.find(' ', start);
		if (delimiterPos != std::string::npos) {
			parse.params.push_back(line.substr(start, delimiterPos - start));
			start = delimiterPos + 1;
		} else {
			parse.params.push_back(line.substr(start));
			break;
		}
	}
	return parse;
}
