/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 08:19:53 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/08 11:14:15 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static std::string	get_names(channel_t channels, std::string channel_name)
{
	std::string	names;

	channel_t::iterator map;
    for (channel_t::iterator iter = channels.begin(); iter != channels.end(); ++iter) {
        std::pair<std::string, std::string> key = iter->first;
        if (key.first == channel_name) {
            map = iter;
        }
    }
	std::vector<User>	users = (*map).second;
	std::vector<User>::iterator	ite = users.begin();
	for (; ite != users.end() - 1; ite++)
		names += (*ite).get_nickname() + " ";
	names += (*ite).get_nickname();
	return (names);
}

std::string	channel_response(channel_t &channels, std::string channel_name, User &user)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER << " 353 " << user.get_nickname()
		<< " = #" << channel_name << " :" << get_names(channels, channel_name) << "\r\n";
	
	ss	<< ":" << SERVER << " 366 " << user.get_nickname()
		<< " #" << channel_name << " :End of NAMES list." << "\r\n";
	return (ss.str());
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
// stiil working on join_channel part
void	join_channel(client_t &clients, size_t i, channel_t &channels, s_list &user)
{
	std::string		reply;
	std::string		channel_name = "";
    std::string		channel_key = "";
	bool			is_there = true;
	while (user._channel.size()) {
		channel_name = user._channel.back();
		if (user._key.size())
			channel_key = user._key.back();
		channel_t::iterator map;
		for (channel_t::iterator iter = channels.begin(); iter != channels.end(); ++iter) {
			std::pair<std::string, std::string> key = iter->first;
			if (key.first == channel_name) {
				map = iter;
				is_there = false;
			}
		}
		if (!check_name(channel_name))
		{
			std::cout << "forbidden character in channel name" << std::endl;
			return ;
		}
		if (is_there)
		{
			std::vector<User> channel_users;
			std::pair<std::string, std::string>	chaine;
			channel_users.push_back(clients[i].second);
			chaine.first = channel_name;
			chaine.second = channel_key;
			channel new_channel(chaine, channel_users);
			channels.insert(new_channel);
			
			reply = msg_format(clients[i].second) + " JOIN #" + channel_name + "\r\n";
			send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
			std::cout << "channel created" << std::endl;
		}
		else // add user to channel
		{
			std::vector<User>::iterator	ite;
			if(map->first.second == channel_key)
			{
				(*map).second.push_back(clients[i].second);
		
				std::vector<User>	users = (*map).second;
				std::vector<User>::iterator	ite = users.begin();
				for (; ite != users.end(); ite++)
				{
					reply = msg_format(clients[i].second) + " JOIN #" + channel_name + "\r\n";
					send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
				}
			} else {
				reply = "475  " + msg_format(clients[i].second) + " :Cannot join channel (+k)\r\n";
				send(clients[i].second.get_fd(), reply.c_str(), reply.length(), 0);
			}
			reply = channel_response(channels, channel_name, clients[i].second);
			send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
		}
		channel_name = "";
		if (user._key.size()) {
			user._key.pop_back();
			channel_key = "";
		}
		user._channel.pop_back();
	}
}
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
	if (user._channel.size())
		join_channel(clients, i, channels, user);
}