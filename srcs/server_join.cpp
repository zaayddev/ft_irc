/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 02:34:58 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/31 06:43:56 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

bool	check_input(std::string nick, client_t &clients, int i, int index)
{
	if (index) {
		if (nick == "")
            return (senderr("NICK", clients[i].second.get_fd(), 431), false);
        if (!validNick(nick))
            return (senderr(nick, clients[i].second.get_fd(), 432), false);
		// for (client_t::iterator it = clients.begin(); it != clients.end(); it++)
		// 	if (nick == it->second.get_nickname())
        //         return (senderr("NICK", clients[i].second.get_fd(), 0), false);
	} else if (!index) {
		int condition = 0;
		int j = 0;
		if (nick == "")
            return (senderr("USER", clients[i].second.get_fd(), 461), false);
		while (nick[j]) {
			while(nick[j] && !isspace(nick[j]))
				j++;
			while(nick[j] && isspace(nick[j]))
				j++;
			condition++;
		}
		if (condition != 4)
		    return (senderr("USER", clients[i].second.get_fd(), 461), false);
	}
	return (true);
}

void	server_join(std::vector<client> &clients, std::string client_msg, int i)
{
	int		index ;

	index = client_msg.length();
	if (!client_msg.find("NICK"))
	{
		if (index > 5 && isspace(client_msg[4]))
		{
			std::string nick = trim(client_msg, 5);
			if (check_input(nick, clients, i, 1))
				clients[i].second.set_nickname(nick);
		}
		else if (clients[i].second.get_nickname().size())
            senderr("NICK", clients[i].second.get_fd(), 2);
		else
            senderr("NICK", clients[i].second.get_fd(), 461);
	}
	else if (!client_msg.find("USER") && clients[i].second.get_nickname().size())
	{
		if (index > 5 && isspace(client_msg[4]))
		{
			std::string _user = trim(client_msg, 5);
			if (check_input(_user, clients, i, 0)) 
				clients[i].second.set_user(trimFront(_user,0));
		}
		else
            senderr("USER", clients[i].second.get_fd(), 461);
	}
	if (clients[i].second.get_nickname().size() && clients[i].second.get_user().size()) {
        clients[i].second.set_is_complete(true);
        welcome_msg(clients[i].second);
    }
}