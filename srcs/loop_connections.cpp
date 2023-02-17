/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_connections.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 02:50:24 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/07 10:17:51 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

void	loop_connections(int socket_fd, std::string password, struct addrinfo *p)
{
	std::vector<client>	clients;
	channel_t			channels;
	pollfd				socket;
	User				server(" ");
	
	socket.fd = socket_fd;
	
	/* 
	 * the events variable is set to POLLIN to tell the system that 
	 * the application is interested in monitoring the socket for incoming data, 
	 * and the revents variable is set to 0, to indicate that no events 
	 * have occurred on the socket yet, at the time of calling poll function.
	 * [ POLLIN is a macro constant defined in the <poll.h> header, 
	 * which represents an event flag that indicates that there is data available to read on the file descriptor. ]
	*/
	
	socket.events = POLLIN;
	socket.revents = 0;
	clients.push_back(client(socket, server));
	while (true) 
	{
		initialise_poll(clients, clients.size());
		accept_call(clients, socket_fd, p);
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (clients[i].first.revents == 0)
				continue;
			if (!(clients[i].first.revents & POLLIN))
				std::cerr << "error; revents problem!!" << std::endl;
			if (i != 0)
			{
				clients[i].second.msg += rcv_msg(clients[i].first.fd, clients, i, channels);
				if (i < clients.size() && clients[i].second.msg.find(END) != npos)
					cmds_parsing(clients, channels, clients[i].second.msg, i, password);
			}
		}
	}
}