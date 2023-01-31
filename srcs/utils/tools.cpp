/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 18:46:44 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/30 23:57:42 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool	check_password(std::string input, std::string pwd, int fd)
{
	std::string ps = trimPass(input, 5);
	if (ps == "" || ! isspace(input[4]))
	{
		std::string reject = reject_msg("PASS", 461);
		send(fd, reject.c_str(), reject.length(), 0);
		return (false);
	}
	if (ps == pwd)
		return (true);
	else
		return (false);
}

std::string getPassword(char *s)
{
    return std::string(s);
}

int getPort(char *s)
{
    if (strlen(s) > 5)
        return 0;
    for (int i = 0; s[i]; i++)
    {
        if(!isdigit(s[i]))
            return (0);
    }
    return (atoi(s));
}

bool	check_input(std::string nick, client_type &clients, int i, int index)
{
	if (index)
	{
		if (nick == "")
		{
			std::string reject = reject_msg("NICK", 461);
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
			return (false);
		}
		for (client_type::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (nick == it->second.get_nickname()) {
				std::string reject = reject_msg(nick, 0);
				send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
				return (false);
			}
		}
	}
	if (!index)
	{
		int condition = 0;
		int j = 0;
		if (nick == "")
		{
			std::string reject = reject_msg("USER", 461);
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
			return (false);
		}
		
		while (nick[j])
		{
			while(nick[j] && !isspace(nick[j]))
				j++;
			while(nick[j] && isspace(nick[j]))
				j++;
			condition++;
		}
		if (condition != 4)
		{
			std::string reject = reject_msg("USER", 461);
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
			return (false);
		}
	}
	return (true);
}


std::string	reject_msg(std::string cmd, int i)
{
	std::stringstream	ss;
	
	if ( i == 461 )
		ss << RED << "461 " << RESET << cmd << " :Not enough parameters\r\n";
    else if (i == 462)
        ss << RED << "462 " << RESET << " :You may not reregister\r\n";
	else if (i == 464)
		ss << RED << "464" << RESET << " :Password incorrect\r\n";

    
	else if (i == 1)
		ss << "Invalid character\r\n";
	else if (i == 2)
		ss << "You already knoen as " << cmd << "\r\n";
	else if (!i)
		ss << "Check [" << cmd << "] already exist\r\n";
	else if (i == -1)
		ss << "Add a parameter : " << "[" << cmd << "] <PARAMETRE>\r\n";
	return (ss.str());
}

std::string	welcome_msg(User user)
{
	std::stringstream	ss;

	ss << "U : " << user.get_nickname() \
		<< "!" << user.get_user() << "@" << user.get_ip() << "\r\n";
	return (ss.str());
}

void    user_authentification(client_type &clients, \
	std::string input, std::string password, size_t i)
{
    if (check_password(input, password, clients[i].second.get_fd()))
        clients[i].second.set_authentification(true);
    else
        std::cout << RED << "[" << clients[i].second.get_ip() << "]" << RESET << " : Password incorrect" << std::endl;
}

void	close_connection(client_type &clients, size_t i)
{
	if (close(clients[i].first.fd) == -1)
		std::cerr << "failed to close file descriptor" << std::endl;
	clients.erase(clients.begin() + i);
}

std::string	getTime()
{
	std::string	h, mi;
    time_t now = time( 0 );
	tm *ltm = localtime( &now );
	h = std::to_string( ltm->tm_hour );
	if ( 1 + ltm->tm_hour < 10 )
		h = '0' + h;
	mi = std::to_string( ltm->tm_min );
	if ( 1 + ltm->tm_min < 10 )
		mi = '0' + mi;
	return (h + ":" + mi);
}

std::string ip_itostr(in_addr_t ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    return std::string(std::to_string(bytes[0]) + "." + std::to_string(bytes[1]) + "." \
        + std::to_string(bytes[2]) + "." + std::to_string(bytes[3]));
}