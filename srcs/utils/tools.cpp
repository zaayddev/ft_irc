/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 18:46:44 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/31 02:56:09 by yelgharo         ###   ########.fr       */
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

int getPort(std::string s)
{
    if (s.length() > 5)
        return 0;
    for (int i = 0; s[i]; i++)
    {
        if(!isdigit(s[i]))
            return (0);
    }
    return (stoi(s));
}

void    user_authentification(client_t &clients, \
	std::string input, std::string password, size_t i)
{
    if (check_password(input, password, clients[i].second.get_fd()))
        clients[i].second.set_authentification(true);
    else
        std::cout << RED << "[" << clients[i].second.get_ip() << "]" \
            << RESET << " : Password incorrect" << std::endl;
}

void	close_connection(client_t &clients, size_t i)
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
    return std::string(std::to_string(ip & 0xFF) + "." \
        + std::to_string((ip >> 8) & 0xFF) \
        + "." + std::to_string((ip >> 16) & 0xFF) \
        + "." + std::to_string((ip >> 24) & 0xFF));
}

std::string trim(std::string s, int i)
{
	std::string ret = "";
	int j = s.length() - 1;

	while (j > i && isspace(s[j]))
		j--;
	while (s[i] && isspace(s[i]))
		i++;
	while (s[i] && i <= j)
		ret += s[i++];
	return (ret);
}

std::string trimFirst(std::string s, int i)
{
	std::string ret = "";

	while (s[i] && isspace(s[i]))
		i++;
	while (s[i] && !isspace(s[i]))
		ret += s[i++];
	return (ret);
}
