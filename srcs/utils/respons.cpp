/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respons.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 23:46:49 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/01 00:45:01 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

std::string prompte()
{
	std::stringstream   ss;
	ss << BLUE << "[" << getTime() << "] " << RST;
	return (ss.str());
}

void	welcome_msg(User user)
{
	std::string s;

	s = "U : " + user.get_nickname() \
		+ "!" + user.get_username() + "@" + user.get_ip() + "\r\n" + prompte();
	send(user.get_fd(), s.c_str(), s.length(), 0);
}

void	nickchange_msg(User user, std::string str)
{
	std::string s;

	s = str + " changed his nickname to " + user.get_nickname() + "\r\n" \
		+ prompte();
	send(user.get_fd(), s.c_str(), s.length(), 0);
}

std::string	reject_msg(std::string cmd, int ern)
{
	std::stringstream	ss;

	if (ern == 431)
		ss << RED << "431" << RST << " :No nickname given\r\n";
	else if (ern == 432)
		ss << RED << "432 " << RST << cmd << " :Erroneus nickname\r\n";
	else if (ern == 433)
		ss << RED << "433 " << RST << cmd << " :Nickname is already in use\r\n";
	else if (ern == 436)
		ss << RED << "436 " << RST << cmd << " :Nickname collision KILL\r\n";
	else if ( ern == 461 )
		ss << RED << "461 " << RST << cmd << " :Not enough parameters\r\n";
	else if (ern == 462)
		ss << RED << "462 " << RST << " :You may not reregister\r\n";
		
					//---
	else if (ern == 464)
		ss << RED << "464" << RST << " :Password incorrect\r\n";
	return (ss.str());
}

void	senderr(std::string cmd, int fd, int ern) {
	std::string reject = reject_msg(cmd, ern) + prompte();
	send(fd, reject.c_str(), reject.length(), 0);
}
