/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeimoun <pmeimoun@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:48:26 by pmeimoun          #+#    #+#             */
/*   Updated: 2026/02/25 12:21:46 by pmeimoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

//constructeur

Command Command::parse(const std::string& line) {
	Command cmd;
	size_t start = 0;
	size_t delimiterPos = 0;
	size_t len = line.length();

	if (!line.empty() && line[0] == ':') {
		delimiterPos = line.find(' ', start);
		if (delimiterPos != std::string::npos) {
			cmd.author = line.substr(1, delimiterPos - 1);
			start = delimiterPos + 1;
		} else {
			cmd.author = line.substr(1);
			return cmd;
		}
	}
	delimiterPos = line.find(' ', start);
	if (delimiterPos != std::string::npos) {
		cmd.request = line.substr(start, delimiterPos - start);
		start = delimiterPos + 1;
	} else {
		cmd.request = line.substr(start);
		return cmd;
	}
	while (start < len) {
		if (line[start] == ':') {
			cmd.params.push_back(line.substr(start + 1));
			break;
		}
		delimiterPos = line.find(' ', start);
		if (delimiterPos != std::string::npos) {
			cmd.params.push_back(line.substr(start, delimiterPos - start));
			start = delimiterPos + 1;
		} else {
			cmd.params.push_back(line.substr(start));
			break;
		}
	}
	return cmd;
}
