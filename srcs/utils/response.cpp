/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 23:46:49 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/12 17:05:00 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void	welcome_msg(User user)
{
	std::string s;

	s = ":" + (std::string)SERVER + " 001 " + user.get_nickname() + " :Welcome to our IRC network! "+ user.get_nickname() \
		+ "!" + user.get_username() + "@" + user.get_ip() + " \r\n";
    s = s + ":" + (std::string)SERVER +  " 002 " + user.get_nickname() + " :We're glad you're here.\r\n";
    s = s + ":" + (std::string)SERVER + " 003 " + user.get_nickname() + " :HAPPY CHATTING...\r\n";  
	send(user.get_fd(), s.c_str(), s.length(), 0);
}

std::string	msg_format(User &user)
{
	std::stringstream	ss;

	ss	<< ":" << user.get_nickname() << \
        "!" << user.get_username() << "@" << user.get_ip();
	return (ss.str());
}

void	nickchange_msg(User user, std::string str)
{
	std::string s;

	s = ":" + str + " NICK " + user.get_nickname() + " :You are kown as " + user.get_nickname() + "\r\n";
	send(user.get_fd(), s.c_str(), s.length(), 0);
}

std::string	reject_msg(std::string cmd, int ern, client_t &clients, int i)
{
	std::stringstream	ss;

	if (ern == 431)
		ss << ern <<" "<< clients[i].second.get_fd() <<": No nickname given\r\n" ;
	else if (ern == 432)
		ss << ern <<" "<< clients[i].second.get_fd() <<": " << cmd << " Erroneus nickname\r\n";
	else if (ern == 433)
		ss << ern <<" "<< clients[i].second.get_fd() <<": " << cmd << " Nickname is already in use\r\n";
	else if (ern == 436)
		ss << ern <<" "<< clients[i].second.get_fd() <<": " << cmd << " Nickname collision KILL\r\n";
	else if ( ern == 461 )
		ss << ern <<" "<< clients[i].second.get_fd() <<": " << cmd << " Not enough parameters\r\n";
	else if (ern == 462)
		ss << ern <<" "<< clients[i].second.get_fd() <<": You may not reregister\r\n";
	else if (ern == 464)
		ss << ern <<" "<< clients[i].second.get_fd() <<": Password incorrect\r\n";
	else if (ern == 403)
		ss << ern <<" "<< clients[i].second.get_fd() <<": " << cmd << " No such channel\r\n";
    else if (ern == 401)
		ss << ern <<" "<< clients[i].second.get_fd() <<": " << cmd << " No such nick/channel\r\n";
	return (ss.str());
}

void	senderr(std::string cmd, int i, client_t &clients, int ern)
{
	std::string reject = reject_msg(cmd, ern, clients, i);
	send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
}
