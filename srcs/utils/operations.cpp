/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 03:02:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/09 16:46:52 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void    ping_message(client_t &clients, size_t i)
{
	std::string reply = "PONG :" + (std::string)SERVER + "\r\n";
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
	reply = reject_msg(take_nickname_from_msg(msg), i, clients, 401);
	send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
}

static void		leave_channel(client_t &clients, size_t i, channel_t &channels, std::string &msg, std::string part_msg)
{
	std::string		reply;
	std::string		channel_name;
	bool			is_there = false;

	msg.erase(0, 1);
	if (msg.find(",", 0) != npos)
	{
		channel_name = msg.substr(0, msg.find(",", 0));
		msg.erase(0, msg.find(",", 0) + 1);
	}
	else
	{
		channel_name = msg.substr(0, msg.find(" ", 0));
		msg.erase(0, msg.find(",", 0));
	}
	if (channel_name.size() == 0)
		return ;
    channel_t::iterator map;
	for (channel_t::iterator iter = channels.begin(); iter != channels.end(); ++iter) {
		std::pair<std::string, std::string> key = iter->first;
		if (key.first == channel_name) {
			map = iter;
			is_there = true;
		}
	}
	if (is_there)
	{
		std::vector<User>::iterator	ite;
		for (ite = (*map).second.begin(); ite != (*map).second.end(); ite++)
		{
			reply = msg_format(clients[i].second) + " PART #" + channel_name + " " + part_msg + "\r\n";
			send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
		}
		for (ite = (*map).second.begin(); ite != (*map).second.end(); ite++)
		{
			if (clients[i].second.get_nickname() == (*ite).get_nickname())
			{
				(*map).second.erase(ite);
				break ;
			}
		}
		if ((*map).second.size() == 0)
		{
			std::cout << "channel deleted" << std::endl;
			channels.erase(map);
		}
	}
	else
	{
		reply = reject_msg("", i, clients, 401);
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
}

/*
	The PART command causes the user sending the message to be removed
	from the list of active members for all given channels listed in the
   	parameter string.
	If a "Part Message" is given, this will be sent
   	instead of the default message, the nickname.
*/

void		leave_channels(client_t &clients, size_t i, channel_t &channels, std::string &msg)
{
	std::string		part_msg;

	if (msg.find(":", 0) != npos)
	{
		part_msg = msg.substr(msg.find(":", 0) + 1, msg.size() - (msg.find(":", 0) + 1));
		msg.erase(msg.find(":", 0) - 1, msg.size() - (msg.find(":", 0) - 1));
	}
	msg.erase(0, 5);
	if (msg.find(",", 0) == npos)
		leave_channel(clients, i, channels, msg, part_msg);
	else
	{
		while (msg.size())
			leave_channel(clients, i, channels, msg, part_msg);
	}
}

bool	channel_msg(client_t &clients, size_t i, channel_t &channels, std::string &msg)
{
	std::string 						reply;
	std::string							channel_name = msg.substr(9, (msg.find(" ", 9) - 9));
	std::pair<std::string, std::string> key = std::make_pair(channel_name, "");
	channel_t::iterator					it = channels.find(key);
	
	if (it != channels.end() && check_user_exist((*it).second, clients[i].second.get_nickname()))
	{
		std::vector<User>			users = (*it).second;
		std::vector<User>::iterator	ite = users.begin();
		for (; ite != users.end(); ite++)
		{
			if (clients[i].second.get_nickname() == (*ite).get_nickname())
				continue ;
			reply = msg_format(clients[i].second) + " " + msg + "\r\n";
			send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
		}
	}
	else
	{
		std::cout << "channel message failed" << std::endl;
		reply = reject_msg("", i, clients, 401);
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	return (false);
}

bool    channel_operations(client_t &clients, channel_t &channels, std::string msg, int i)
{
	std::string	reply;

	if (!msg.find("PING "))
		ping_message(clients, i);
	else if (!msg.find("PRIVMSG #"))
		channel_msg(clients, i, channels, msg);
	else if (!msg.find("PRIVMSG "))
		priv_msg(clients, i, msg);
	else if (!msg.find("JOIN "))
		join_channels(clients, i, channels, msg);
    else if (!msg.find("PART"))
		leave_channels(clients, i, channels, msg);
	else if (!msg.find("ARTIST "))
		bot(clients, i, msg);
	return (false);
}
