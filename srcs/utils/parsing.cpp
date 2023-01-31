/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 04:17:49 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/31 00:09:31 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void	cmds_parsing(client_type &clients, channel_type &channels, \
    std::string &msg, int i, std::string password)
{
    std::string tmp;
    (void) channels;
	for (std::string::size_type	pos = msg.find("\r\n"); pos != std::string::npos; pos = msg.find("\r\n"))
    {
		if (pos == 0)
			std::cout << YELLOW << "WARNING" << RESET <<std::endl;
        tmp = msg.substr(0, pos);
		msg.erase(0, pos + 2);
        if (!clients[i].second.get_is_complete())
        {
            if (!tmp.find("PASS"))
                user_authentification(clients, tmp, password, i);
            else if (clients[i].second.get_authentification())
				server_join(clients, tmp, i);
        }
        else if (clients[i].second.get_is_complete())
        {
            if (!tmp.find("PASS"))
            {
                std::string reject = reject_msg("PASS", 462);
                send(clients[i].second.get_fd(), reject.c_str(), reject.length(), 0);
            }
            else
		    {
			    if (channel_operations(clients, channels, tmp, i))
				    break;
	    	}
        }
	}
}
