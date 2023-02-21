/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 18:46:44 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/21 09:54:55 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void    tolowstr(std::string &s)
{
    for (int i = 0; s[i]; i++)
        s[i] = tolower(s[i]);
}

int getPort(char* s)
{
	if (strlen(s) > 5)
		return 0;
	for (int i = 0; s[i]; i++)
	{
		if (!isdigit(s[i]))
			return (0);
	}
	return (atoi(s));
}

bool	check_user_exist(std::set<User> users, std::string nick)
{
	for (std::set<User>::iterator it_users = users.begin(); it_users != users.end(); it_users++)
	{
		if (it_users->get_nickname() == nick)
			return (true);
	}
	return (false);
}

std::string ip_itostr(struct addrinfo  *addr_info)
{
	std::stringstream ss;
    char ip_str[INET6_ADDRSTRLEN];

    if (addr_info->ai_family == AF_INET) // IPV4
	{
        struct sockaddr_in* ipv4 = reinterpret_cast<struct sockaddr_in*>(addr_info->ai_addr);
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, INET_ADDRSTRLEN);

    } else if (addr_info->ai_family == AF_INET6) // IPV6
	{
        struct sockaddr_in6* ipv6 = reinterpret_cast<struct sockaddr_in6*>(addr_info->ai_addr);
        inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip_str, INET6_ADDRSTRLEN);
    } else
        return "FAILED";
    
    ss << ip_str;
    return ss.str();
}


std::string trim(std::string s, int i)
{
	int			j;
	std::string ret;

	ret = "";
	j = s.length() - 1;
	while (j > i && isspace(s[j]))
		j--;
	while (s[i] && isspace(s[i]))
		i++;
	while (s[i] && i <= j)
		ret += s[i++];
	return (ret);
}

std::string trimFront(std::string s, int i)
{
	std::string ret;
	
	ret = "";
	while (s[i] && isspace(s[i]))
		i++;
	while (s[i] && !isspace(s[i]))
		ret += s[i++];
	return (ret);
}

std::string	take_nickname_from_msg(std::string msg)
{
	int	i = 0;
	std::string	nick;

	for (; msg[i] != ' '; i++)
		continue ;
	i++;
	for (; msg[i] != ' '; i++)
		nick.push_back(msg[i]);
	return (nick);
}
