/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 00:20:06 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/05 12:10:31 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool	check_password(std::string input, std::string pwd, int fd)
{
	std::string ps = trim(input, 5);
	if (ps == "" || ! isspace(input[4]))
		return (senderr("PASS", fd, 461), false);
	if (ps == pwd)
		return (true);
	return (false);
}

void	user_authentification(client_t &clients, \
	std::string input, std::string password, size_t i)
{
	if (check_password(input, password, clients[i].second.get_fd()))
		clients[i].second.set_authentification(true);
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