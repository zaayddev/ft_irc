/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 03:02:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/01 17:16:21 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void    ping_message(client_t &clients, size_t i)
{
	std::string reply = "PONG " + (std::string)SERVER + "\r\n";
	send(clients[i].second.get_fd(), reply.c_str(), reply.length(), 0);
	std::cout << "channel_operations:: ";
}

void	priv_msg(client_t &clients, size_t i, std::string &msg)
{
	std::string reply;

	for (client_t::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it).second.get_nickname() == take_nickname_from_msg(msg))
		{
			reply = msg_format(clients[i].second) + " " + msg + "\r\n";
			send((*it).first.fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}
	std::cout << "sending private message failed" << std::endl;
	reply = reject_msg(take_nickname_from_msg(msg), 401);
	send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
}

bool	check_name(std::string input)
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		if (isalnum(input[i]) == 0)
		{
			std::cout << "Nick/Channel name contains invalid chars!" << std::endl;
			return (false);
		}
	}
	return (true);
}

void	join_channel(client_t &clients, size_t i, channel_t &channels, std::string &msg)
{
	std::string		reply;
	std::string		channel_name;
	(void) msg;

	channel_t::iterator it = channels.find(channel_name);

	if (it == channels.end())
	{
		std::vector<User> channel_users;

		channel_users.push_back(clients[i].second);
		channel new_channel(channel_name, channel_users);
		channels.insert(new_channel);
		
		reply = msg_format(clients[i].second) + " JOIN #" + channel_name + "\r\n";
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
		std::cout << "channel created" << std::endl;
	}
	else // add user to channel
	{
		(*it).second.push_back(clients[i].second);

		std::vector<User>	users = (*it).second;
		std::vector<User>::iterator	ite = users.begin();
		for (; ite != users.end(); ite++)
		{
			reply = msg_format(clients[i].second) + " JOIN #" + channel_name + "\r\n";
			send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
		}
	}
}

void	join_channels(client_t &clients, size_t i, channel_t &channels, std::string &msg)
{	
	if (msg.find(",", 0) == npos)
		join_channel(clients, i, channels, msg);
	else {
		while (msg.size())
			join_channel(clients, i, channels, msg);
	}
}

bool    channel_operations(client_t &clients, channel_t &channels, std::string msg, int i)
{
	std::string	reply;
    (void) channels;
	if (!msg.find("PING "))
		ping_message(clients, i);
	else if (!msg.find("PRIVMSG "))
		priv_msg(clients, i, msg);
	else if (!msg.find("JOIN #"))
		join_channels(clients, i, channels, msg);
	return (false);
}
