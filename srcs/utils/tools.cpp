/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 18:46:44 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/29 20:25:18 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool	check_password(std::string input, std::string pwd)
{
	if (input.substr(5, input.size() - 5) == pwd)
		return (true);
	else
		return (false);
}

std::string	reject_msg(std::string user, int i) {
	std::stringstream	ss;
	
	if (!i)
		ss << "Please check " << user << " already exist\r\n";
	else if (i == 1)
		ss << "Invalid character\r\n";
	else if (i == -1)
		ss << "Please add a parameter : " << "[" << user << "] <PARAMETRE>\r\n";
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
    if (check_password(input, password) == true)
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

bool	check_input(std::string nick, client_type &clients, int fd)
{
	for (client_type::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (nick == it->second.get_nickname()) {
			std::string reject = reject_msg(nick, 0);
			send(fd, reject.c_str(), reject.length(), 0);
			return (false);
		}
	}
	for (int i = 0; nick[i] != '\0'; i++)
	{
		if (!isalnum(nick[i]))
		{
			std::string reject = reject_msg(nick, 1);
			send(fd, reject.c_str(), reject.length(), 0);
			return (false);
		}
	}
	return (true);
}
