/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 00:20:06 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/16 14:13:59 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool	check_password(std::string input, std::string pwd, int i, client_t &clients)
{
	std::string ps = trim(input, 5);
	if (ps == "" || ! isspace(input[4]))
		return (senderr("PASS", i, clients, 461), false);
	if (ps == pwd)
		return (true);
	return (false);
}

void	user_authentification(client_t &clients, \
	std::string input, std::string password, size_t i)
{
	if (check_password(input, password, i, clients)) {
		clients[i].second.set_authentification(true);
		std::cout << GREEN_BOLD << "[" << clients[i].second.get_ip() << "]" \
			<< RST << " : Password correct" << std::endl;
	}
	else
		std::cout << RED << "[" << clients[i].second.get_ip() << "]" \
			<< RST << " : Password incorrect" << std::endl;
}

bool	validNick(std::string s)
{
	int i;
	if (s.find("NICK") != npos)
		return (false);
	for (i = 0; s[i]; i++)
		if (!isdigit(s[i]) && !islower(s[i]) && s[i] != '.' 
			&& s[i] != '-' && s[i] != '_')
				return (false);
	if (i > 30)
		return (false);
	return (true);
}

void	close_connection(client_t &clients, size_t i)
{
	if (close(clients[i].first.fd) == -1)
		std::cerr << "failed to close file descriptor" << std::endl;
	clients.erase(clients.begin() + i);
}

void	close_connection(client_t &clients, client_t::iterator it)
{
	if (close((*it).second.get_fd()) == -1)
		std::cerr << "failed to close file descriptor" << std::endl;
	clients.erase(it);
}

int	first_in_check(char* s2, std::string s3)
{
	int		port;
	size_t	i;

	i = 0;
	port = getPort(s2);
	if (port < 1 || port > 65535)
		return (std::cerr << RED_BOLD << "ERROR;\n\tInvalid port number" << \
			RST << std::endl, 0);
	while (s3[i])
	{
		if (!isspace(s3[i]))
			break ;
		i++;
	}
	if(s3.length() == i)
		return (std::cerr << RED_BOLD << "ERROR;\n\tInvalid password" << RST \
			<< std::endl, 0);
	return (port);
	
}