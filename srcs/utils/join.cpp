/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 08:19:53 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/06 18:42:58 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

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
// stiil working on join_channel part
/*void	join_channel(client_t &clients, size_t i, channel_t &channels, s_list &user)
{
	std::string		reply;
	std::string		channel_name;
	channel_t::iterator it = channels.find(channel_name);

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
	if (!check_name(channel_name))
	{
		std::cout << "forbidden character in channel name" << std::endl;
		return ;
	}
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
}*/

void	join_channels(client_t &clients, size_t i, channel_t &channels, std::string &msg)
{

	s_list user;
	std::string channel = "";
	std::string key = "";
	msg.erase(0, 5);
	size_t l = msg.find(' ');
	size_t t = msg.find('&');
	size_t p = msg.find('#');
	if (l != npos &&( t != npos || p != npos)){
		channel = msg.substr(0, l);
		key = msg.substr(l + 1, msg.length() - 1);
		msg.erase(0, msg.length());
	} else if ( t != npos || p != npos) {
		channel = msg;
		msg.erase(0, msg.length());
	}
	while (channel.size())
	{
		size_t r = channel.find(",", 0);
		if (r != npos) {
			user._channel.push_front(channel.substr(1, r - 1));
			channel.erase(0,  r + 1);
		} else {
			user._channel.push_front(channel.substr(1, channel.length()));
			channel.erase(0,  channel.length());
		}
	}
	while (key.size()) {
		size_t r = key.find(",", 0);
		if (r != npos) {
			user._key.push_front(key.substr(0, r));
			key.erase(0, r + 1);
		} else {
			user._key.push_front(key.substr(0, key.length()));
			key.erase(0,  key.length());
		}
	}
	return;
	// if (user._channel.size())
	// 	join_channel(clients, i, channels, user);
}