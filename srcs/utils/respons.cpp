/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respons.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 23:46:49 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/01 09:47:43 by yelgharo         ###   ########.fr       */
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

	s = ":ft_irc NOTICE AUTH :" + user.get_nickname() \
		+ "!" + user.get_username() + "@" + user.get_ip() + "\n";
	send(user.get_fd(), s.c_str(), s.length(), 0);
}

void	nickchange_msg(User user, std::string str)
{
	std::string s;

	s = ":ft_irc NOTICE AUTH :" + str + " changed his nickname to " + user.get_nickname() \
        + "\n";
	send(user.get_fd(), s.c_str(), s.length(), 0);
}

std::string	reject_msg(std::string cmd, int ern)
{
	std::stringstream	ss;

	if (ern == 431)
		ss << ":ft_irc ERROR :" << "431" << " :No nickname given\n" ;
	else if (ern == 432)
		ss << ":ft_irc ERROR :" << "432 " << cmd << " :Erroneus nickname\n";
	else if (ern == 433)
		ss << ":ft_irc ERROR :" << "433 " << cmd << " :Nickname is already in use\n";
	else if (ern == 436)
		ss << ":ft_irc ERROR :" << "436 " << cmd << " :Nickname collision KILL\n";
	else if ( ern == 461 )
		ss << ":ft_irc ERROR :" << "461 " << cmd << " :Not enough parameters\n";
	else if (ern == 462)
		ss << ":ft_irc ERROR :" << "462 " << " :You may not reregister\n";
		
					//---
	else if (ern == 464)
		ss << ":ft_irc ERROR :" << "464" << " :Password incorrect\n";
	return (ss.str());
}

void	senderr(std::string cmd, int fd, int ern) {
	std::string reject = reject_msg(cmd, ern);
	send(fd, reject.c_str(), reject.length(), 0);
}
