/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 04:17:49 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/31 03:12:42 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void	cmds_parsing(client_t &clients, channel_t &channels, \
    std::string &msg, int i, std::string password)
{
    std::string tmp;
    (void) channels;
	for (size_t	pos = msg.find(END); pos != npos; pos = msg.find(END))
    {
		if (pos == 0)
			std::cout << YELLOW << "WARNING" << RESET <<std::endl;
        tmp = msg.substr(0, pos);
		msg.erase(0, pos + 2);
        if (!clients[i].second.get_is_complete()) {
            if (!tmp.find("PASS"))
                user_authentification(clients, tmp, password, i);
            else if (clients[i].second.get_authentification())
				server_join(clients, tmp, i);
        } else if (clients[i].second.get_is_complete()) {
            if (!tmp.find("PASS"))
                senderr("PASS", clients[i].second.get_fd(), 462);
            else
			    if (channel_operations(clients, channels, tmp, i))
				    break;
        }
	}
}
