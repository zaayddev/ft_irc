/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 18:46:44 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/05 12:09:42 by yelgharo         ###   ########.fr       */
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

bool	check_user_exist(std::vector<User> users, std::string nick)
{
	for (std::vector<User>::iterator it_users = users.begin(); it_users != users.end(); it_users++)
	{
		if (it_users->get_nickname() == nick)
			return (true);
	}
	return (false);
}

std::string ip_itostr(in_addr_t ip)
{
    std::stringstream   ss;

    ss << (ip & 0xFF)  << "." << ((ip >> 8) & 0xFF) << "." \
        << ((ip >> 16) & 0xFF) << "." << ((ip >> 24) & 0xFF);
	return (ss.str());
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