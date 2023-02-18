/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:46:20 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/18 15:22:35 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool    is_there(User &user, std::string &name) {
std::cout << "is here in target 1" << std::endl;
    std::vector<std::string>::iterator it = user._owned_channels.begin();
	for(; it != user._owned_channels.end(); ++it) {
        if (*it == name) {
            std::cout << "is here in target 2" << std::endl;
			return true;
        }
	}
std::cout << "is here in channel 3" << std::endl;
    return false;
}

static bool if_target(client_t &clients, std::string &target) {
std::cout << "is here in target 1" << std::endl;
    client_t::iterator it = clients.begin();
    for(; it  != clients.end(); ++it) {
        if (it->second.get_nickname() == target) {
            std::cout << "is here in target 2" << std::endl;
            return true;
        }
    }
    std::cout << "is here in target 3" << std::endl;
    return false;
}

// bool    if_target(std::vector<User> users, std::string nick)
// {
//     for (std::vector<User>::iterator it_users = users.begin(); it_users != users.end(); it_users++)
//     {
//         if (it_users->get_nickname() == nick)
//             return (true);
//     }
//     return (false);
// }

// needs is to give/take privilege

static int	if_or_not(client_t &clients, channel_t &channels, std::string &name, std::string &target) {
	int result = 0;

	std::vector<User>::iterator it;
    channel_t::iterator map;
	
	for (channel_t::iterator iter = channels.begin(); iter != channels.end(); ++iter) {
		std::pair<std::string, std::string> key = iter->first;
		if (key.first == name) {
            map = iter;
			it = (*iter).second.begin();
			for(; it != (*iter).second.end(); ++it) {
				if ((*it).get_nickname() == target) {
					std::vector<std::string>::iterator i = (*it)._owned_channels.begin();
					for (; i != (*it)._owned_channels.end(); ++i) {
						if ((*i) == name) {
							return 3;
						}
					} if (i == (*it)._owned_channels.end() && !result) {
						result = 2;
                    	break ;
					}
				}
				if (result == 2)
					break;
			}
			if (result == 2)
				break;
		}
	}
	if (result == 2 || !result) {
        client_t::iterator iti = clients.begin();
        for (; iti != clients.end(); ++iti)
            if ((*it).get_nickname() == (*iti).second.get_nickname()) {
		        (*iti).second._owned_channels.push_back(name);
                break;   
            }
        if (result == 0) {
            (*map).second.push_back((*iti).second);
            std::vector<User>	users = (*map).second;
            std::vector<User>::iterator	ite = users.begin();
            for (; ite != users.end(); ite++)
            {
                std::string reply = msg_format((*iti).second) + " JOIN #" + name + "\r\n";
                send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
            }
        }
    }
	return result;
	
}

void	omode(client_t &clients, size_t i, channel_t &channels, std::string &msg, std::string &name) 
{
	std::string target = trimFront(msg, 0);
    bool there  = is_there(clients[i].second, name);
    std::cout << "--------------------------------" << std::endl;
    bool there_two = if_target(clients, target);
	int r = if_or_not(clients, channels, name, target);
	
	if (there && there_two) {
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
        try {
            std::string md = "you are not operator on this channel or the target not valid user or not valid channel";
            std::cout << md << std::endl;
        } catch (...) {
		    std::cout << "rak tm" << std::endl;
            return ;
        }
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
