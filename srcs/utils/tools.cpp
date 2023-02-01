/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 18:46:44 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/01 13:03:58 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"


int getPort(std::string s)
{
	if (s.length() > 5)
		return 0;
	for (int i = 0; s[i]; i++)
	{
		if (!isdigit(s[i]))
			return (0);
	}
	return (stoi(s));
}

std::string	getTime()
{
	tm		*ltm;
	time_t	now;

	now = time(0);
	ltm = localtime( &now );
	return (std::string((1 + ltm->tm_hour) < 10 ? '0' \
		+ std::to_string(ltm->tm_hour) : std::to_string(ltm->tm_hour)) \
		+ ":" +std::string((1 + ltm->tm_min) < 10 ? '0' \
		+ std::to_string(ltm->tm_min) : std::to_string(ltm->tm_min)));
}

std::string ip_itostr(in_addr_t ip)
{
	return (std::string(std::to_string(ip & 0xFF) + "." \
		+ std::to_string((ip >> 8) & 0xFF) \
		+ "." + std::to_string((ip >> 16) & 0xFF) \
		+ "." + std::to_string((ip >> 24) & 0xFF)));
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