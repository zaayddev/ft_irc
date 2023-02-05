/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 03:02:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/05 16:39:39 by yelgharo         ###   ########.fr       */
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
	reply = reject_msg(take_nickname_from_msg(msg), i, clients, 401);
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
}

void	join_channels(client_t &clients, size_t i, channel_t &channels, std::string &msg)
{	
	msg.erase(0, 5);
	if (msg.find(",", 0) == npos)
		join_channel(clients, i, channels, msg);
	else {
		while (msg.size())
			join_channel(clients, i, channels, msg);
	}
}

void	create_env_file(std::string name)
{
	std::ofstream	ofs(".env");
	std::string	var1 = "CLIENT_ID = \"cfde0e10b10247f194386e2f5d2d375f\"";
	std::string	var2 = "CLIENT_SECRET = \"6256e3fbb10e417eb5d7f55be82b81bc\"";
	std::string	var3 = "ARTIST_NAME = \"" + name + "\"";
	ofs << var1 << std::endl << var2 << std::endl << var3 << std::endl;
	ofs.close();
}

void	handle_bot_cmd(client_t &clients, size_t i, std::string &name)
{	
		// done
        create_env_file(trim(name, 6));
		std::string line;
		system("python ./srcs/utils/main.py");
        std::ifstream   ifs("songs");
        while (getline(ifs, line)) {
			std::string msg = "PRIVMSG " + clients[i].second.get_nickname() + " :♪ " + line + "\r\n";
   	 	    send(clients[i].second.get_fd(), msg.c_str(), msg.length(), 0);
		}
		ifs.close();
		system("rm -f songs .env");
}

bool    channel_operations(client_t &clients, channel_t &channels, std::string msg, int i)
{
	std::string	reply;

	if (!msg.find("PING"))
		ping_message(clients, i);
	else if (!msg.find("PRIVMSG"))
		priv_msg(clients, i, msg);
	//else if (!msg.find("PRIVMSG #"))
		//channel_msg(clients, i, channels, msg);
	else if (!msg.find("JOIN"))
		join_channels(clients, i, channels, msg);
	else if (!msg.find("ARTIST "))
		handle_bot_cmd(clients, i, msg);
	return (false);
}
