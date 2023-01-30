/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 18:46:44 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/30 01:09:38 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool	check_password(std::string input, std::string pwd, int fd)
{
	std::string ps = trimPass(input, 5);
	if (ps == "" || ! isspace(input[4])) {
		std::string reject = reject_msg("PASS", -1);
		send(fd, reject.c_str(), reject.length(), 0);
		return (false);
	}
	if (ps == pwd)
		return (true);
	else {
		std::string reject = reject_msg("PASS", -2);
		send(fd, reject.c_str(), reject.length(), 0);
		return (false);
	}
}

bool	check_input(std::string nick, client_type &clients, int i, int index)
{
	for (int j = 0; nick[j]; j++)
	{
		if (!isalnum(nick[j]))
		{
			std::string reject = reject_msg(nick, 1);
			std::cout << reject << std::endl;
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
			return (false);
		}
	}
	if (index)
	{
		if (nick == "") {
			std::string reject = reject_msg("NICK", -1);
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
		if (nick == "") {
			std::string reject = reject_msg("USER", -1);
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
			return (false);
		}
		for (client_type::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (nick == it->second.get_user()) {
				std::string reject = reject_msg(nick, 0);
				send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
				return (false);
			}
		}
	}
	return (true);
}


std::string	reject_msg(std::string user, int i) {
	std::stringstream	ss;
	
	if (i == 1)
		ss << "Invalid character\r\n";
	else if (i == 2)
		ss << "You already knoen as " << user << "\r\n";
	else if (!i)
		ss << "Check [" << user << "] already exist\r\n";
	else if (i == -1)
		ss << "Add a parameter : " << "[" << user << "] <PARAMETRE>\r\n";
	else if (i == -2)
		ss << "Invalid password\r\n";
	return (ss.str());
}

std::string	welcome_msg(User user) {
	std::stringstream	ss;

	ss << "Welcome to our ft_irc " << user.get_nickname() \
		<< " " << user.get_user() << " " << user.get_ip() << "\r\n";
	return (ss.str());
}

void    user_authentification(client_type &clients, \
	std::string input, std::string password, size_t i) {
    if (check_password(input, password, clients[i].second.get_fd()))
        clients[i].second.set_authentification(true);
    else
        std::cout << "Password invalide" << std::endl;
}

void	close_connection(client_type &clients, size_t i)
{
	if (close(clients[i].first.fd) == -1)
		std::cerr << "failed to close file descriptor" << std::endl;
	clients.erase(clients.begin() + i);
}