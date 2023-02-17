/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:46:20 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/16 22:04:42 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool    is_there(User &user, std::string &name) {
    std::vector<std::string>::iterator it = user._owned_channels.begin();
	for(; it != user._owned_channels.end(); ++it) {
		std::cout << "chanel operators : " << *it << std::endl;
        if (*it == name)
			return true;
	}
    return false;
}
// needs is to give/take privilege

static int	if_or_not(client_t &clients, channel_t &channels, std::string &name, std::string &target) {
	int result = 0;
	std::vector<User>::iterator it;
	
	for (channel_t::iterator iter = channels.begin(); iter != channels.end(); ++iter) {
		std::pair<std::string, std::string> key = iter->first;
		if (key.first == name) {
			it = (*iter).second.begin();
			for(; it != (*iter).second.end(); ++it) {
				if ((*it).get_nickname() == target) {
					std::vector<std::string>::iterator i = (*it)._owned_channels.begin();
					for (; i != (*it)._owned_channels.end(); ++i) {
						if ((*i) == name) {
							result = 3;
							break ;
						}
					}
					if (i == (*it)._owned_channels.end() && !result) {
						result = 2;
                        std::cout << "hello" << std::endl;
                    	break ;
					}
				}
				if (result != 0)
					break;
			}
			if (result != 0)
				break;
		}
	}
	if (result == 2) {
        for (client_t::iterator iti = clients.begin(); iti != clients.end(); ++iti)
            if ((*it).get_nickname() == (*iti).second.get_nickname())
		        (*iti).second._owned_channels.push_back(name);
        std::cout << name << std::endl;
    }
	return result;
	
}

void	omode(client_t &clients, size_t i, channel_t &channels, std::string &msg, std::string &name) 
{
	(void) channels;
	std::string target = trimFront(msg, 0);
	int r = if_or_not(clients, channels, name, target);
	
	if (is_there(clients[i].second, name)) {
		if (r == 3) {
			// inform the client that the target already has the operater privileges ont the channel;
			std::cout << "the target is already +o" << std::endl;
		}
		if (r == 2) {
			// inform the client that the target gain operator privileges;
			std::cout << "now the target is +o" << std::endl;
		}
		if (r == 0) {
			// add the target to channel with the operater privileges;
			std::cout << "the target has joined the channel with +o" << std::endl;
		}
		
	} else {
		// inform the client that he has not the operater privileges on the channel;
		std::cout << "you are not operator on this channel" << std::endl;
	}
	return;
}

void	o_mode(client_t &clients, size_t i, channel_t &channels, std::string &msg, std::string &name) 
{
	(void) channels;
	bool	is_o = false;
	std::cout << msg << std::endl;
	std::vector<std::string>::iterator it = clients[i].second._owned_channels.begin();
	for(; it != clients[i].second._owned_channels.end(); ++it) {
		if (*it == name)
			is_o = true;  
	}
	if (is_o) {
		// do something
	} else {
		// do something else
	}
	return;
}
