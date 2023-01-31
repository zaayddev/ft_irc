/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 02:34:58 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/30 23:56:11 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

std::string trimPass(std::string toTrim, int i)
{
	std::string trimed = "";
	int j = toTrim.length() - 1;

	while (j > i && isspace(toTrim[j]))
		j--;
	while (toTrim[i] && isspace(toTrim[i]))
		i++;
	while (toTrim[i] && i <= j)
	{
		trimed += toTrim[i];
		i++;
	}
	return (trimed);
}

std::string totrim(std::string toTrim, int i)
{
	std::string trimed = "";

	while (toTrim[i] && isspace(toTrim[i]))
		i++;
	while (toTrim[i] && !isspace(toTrim[i]))
	{
		trimed += toTrim[i];
		i++;
	}
	return (trimed);
}


void	server_join(std::vector<client> &clients, std::string client_msg, int i)
{
	int		index ;

	index = client_msg.length();
	if (!client_msg.find("NICK"))
	{
		if (index > 5 && isspace(client_msg[4]))
		{
			std::string nick = totrim(client_msg, 5);
			if (check_input(nick, clients, i, 1))
				clients[i].second.set_nickname(nick);
		}
		else if (clients[i].second.get_nickname().size())
		{
			std::string reject = reject_msg(clients[i].second.get_nickname(), 2);
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
		}
		else
		{
			std::string reject = reject_msg("NICK", 461);
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
		}
	}
	else if (!client_msg.find("USER") && clients[i].second.get_nickname().size())
	{
		if (index > 5 && isspace(client_msg[4]))
		{
			std::string _user = trimPass(client_msg, 5);
			if (check_input(_user, clients, i, 0)) 
				clients[i].second.set_user(totrim(_user,0));
		}
		else
		{
			std::string reject = reject_msg("USER", 461);
			send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
		}
	}
	if (clients[i].second.get_nickname().size() && clients[i].second.get_user().size())
	{
		clients[i].second.set_is_complete(true);
		std::string	welcome = welcome_msg(clients[i].second);
		send(clients[i].second.get_fd(), welcome.c_str(), welcome.length(), 0);
	}
}