/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 03:02:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/01 09:31:45 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void    ping_message(client_t &clients, size_t i)
{
	std::string reply = "PONG " + (std::string)SERVER + "\r\n";
	send(clients[i].second.get_fd(), reply.c_str(), reply.length(), 0);
	std::cout << "channel_operations:: ";
}

bool    channel_operations(client_t &clients, channel_t &channels, std::string msg, int i)
{
	std::string	reply;
    (void) channels;
	if (!msg.find("PING"))
		ping_message(clients, i);
	return (false);
}