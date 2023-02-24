/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 03:02:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/24 17:42:11 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void    ping_message(client_t &clients, size_t i)
{
	std::string reply = "PONG :" + (std::string)SERVER + "\r\n";
	send(clients[i].second.get_fd(), reply.c_str(), reply.length(), 0);
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
        clients[i].second._condition -= 1;
		std::set<User>::iterator	ite;
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
		std::set<User>			users = (*it).second;
		std::set<User>::iterator	ite = users.begin();
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
// 0 1 2 3
// M O D E #mychannel (+/-)o/b someuser

void	mode(client_t &clients, size_t i, channel_t &channels, std::string &msg) {
	msg.erase(0,5);
	if (msg[0] == '#') {
		std::string channel = msg.substr(1, msg.find(' ') - 1);
		msg.erase(0,  msg.find(' ') + 1);
		if (msg[0] == '+' && msg[1] == 'o' && msg[2] == ' ') {
			msg.erase(0,  3);
            omode(clients, i, channels, msg, channel);
		} else if (msg[0] == '-' && msg[1] == 'o' && msg[2] == ' ') {
            msg.erase(0,  3);
			o_mode(clients, i, msg, channel);
		}  else if (msg[0] == 'b' && msg[1] == ' ') {
            msg.erase(0, 2);
			b_mode(clients, i, channels, msg, channel);
		}
	}
}

void    kick(client_t &clients, int &i, channel_t &channels, std::string &msg) {
    
    msg.erase(0, msg.find(' ') + 1);
    std::string channel_name = trimFront(msg, 0);
    msg.erase(0, msg.find(' ') + 1);
    std::string target_name = trimFront(msg,0);
    if (is_there(clients[i].second, channel_name)) {
        for (size_t r = 0; r < clients.size(); r++) {
            if (clients[r].second.get_nickname() == target_name)
                leave_channel(clients, r, channels, channel_name += " ", "kicked");
        }
    } else {
        //you are not allowed to kick no one a l3zaaaawiiiiii
        senderr(msg.substr(0, msg.find(" ")), i, clients, 482);
    }
}

void    names(client_t &clients, size_t i, channel_t &channels, std::string &msg) {
    msg.erase(0, 7);
    std::string tmp = channel_response(channels, msg, clients[i].second);
    if (tmp.size())
        send(clients[i].first.fd, tmp.c_str(), tmp.length(), 0);
}

void    list(client_t &clients, size_t i, channel_t &channels) {
    (void)i;
    std::string channels_name = "";
    std::string clients_name = "";
    channel_t::iterator it = channels.begin();
    if (channels.size()) {
        for (size_t r = 0; r < channels.size() - 1; r++) {
            channels_name += it->first.first + " ";
            it++;
        }
        channels_name += it->first.first;
    }
    size_t r;
    for ( r = 0; r < clients.size() - 1; r++) {
        if (!clients[r].second._condition)
            clients_name += clients[r].second.get_nickname() + " ";
    }
    if (!clients[r].second._condition)
        clients_name += clients[r].second.get_nickname();
    
    std::cout <<    "clients_name : " << clients_name << std::endl;
    std::cout << "channels_name : " << channels_name << std::endl;
}

int32_t check(client_t &clients, std::string msg, int i) {
	if (msg.find("JOIN #") == 0) {
		if (msg.length() == 6) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 1;
	}
	else if (msg.find("ARTIST ") == 0) {
		if (msg.length() == 7) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 2;
	}
	else if (msg.find("PART :") == 0 || msg.find("PART ") == 0) {
		if (msg.length() <= 6) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
            std::cout << msg << std::endl;
			return 0;
		}
		else 
			return 3;
	}
	else if (msg.find("PRIVMSG #") == 0) {
		if (msg.length() == 9) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 4;
	}
	else if (msg.find("PRIVMSG ") == 0) {
		if (msg.length() == 8) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 5;
	}
	else if (msg.find("MODE") == 0) {
		if (msg.length() == 4) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 6;
	}
	else if (msg.find("PING ") == 0 || msg.find("PONG ") == 0) {
		if (msg.length() == 5) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 7;
	}
    else if (msg.find("FILE UP") == 0) {
		if (msg.length() == 7) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 8;
	}
    else if (msg.find("FILE DOWN") == 0) {
		if (msg.length() == 9) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 9;
	}
    else if (msg.find("FILE LIST") == 0) {
		if (msg.length() != 9) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 10;
	}
    else if (msg.find("QUIT") == 0) {
		if (msg.length() > 4) {
            if (msg[4] == ' ')
                return 11;
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 11;
    }
    else if (msg.find("KICK ") == 0) {
		if (msg.length() == 5) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 12;
    }
    else if (msg.find("NAMES ") == 0) {
		if (msg.length() == 6) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 13;
    }
    else if (msg.find("LIST") == 0) {
		if (msg.length() != 4) {
			senderr(msg.substr(0, msg.find(" ")), i, clients, 461);
			return 0;
		}
		else 
			return 14;
    }
	else
		senderr(msg.substr(0, msg.find(" ")), i, clients, 421);
	return 0;
}


bool	channel_operations(client_t &clients, channel_t &channels, std::string msg, int i)
{
    std::cout << msg << std::endl;
	std::string	reply;
	int n = check(clients, msg, i);

	if (n == 1)
		join_channels(clients, i, channels, msg);
	else if (n == 2)
		bot(clients, i, msg);
	else if (n == 3)
		leave_channels(clients, i, channels, msg);
	else if (n == 4)
		channel_msg(clients, i, channels, msg);
	else if (n == 5)
		priv_msg(clients, i, msg);
    else if (n == 6)
	    mode(clients, i, channels ,msg);
	else if (n == 7)
		ping_message(clients, i);
    else if (n == 8)
		transfer(clients, i, n, msg);
    else if (n == 9)
		transfer(clients, i, n, msg);
    else if (n == 10)
		transfer(clients, i, n, msg);
    else if (n == 12)
		kick(clients, i, channels, msg);
    else if (n == 13)
		names(clients, i, channels, msg);
    else if (n == 14)
		list(clients, i, channels);
	return (false);
}