/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:46:20 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/21 15:50:21 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

static bool    is_there(User &user, std::string &name) {
    std::set<std::string>::iterator it = user._owned_channels.find(name);
	if (it != user._owned_channels.end()) {
			return true;
	}
    return false;
}

static bool if_target(client_t &clients, std::string &target) {
    client_t::iterator it = clients.begin();
    for(; it  != clients.end(); ++it) {
        if (it->second.get_nickname() == target)
            return true;
    }
    return false;
}

// static bool if_banded(User &user, std::string &name)
// {
//      std::set<std::string>::iterator it = user._banded_channels.find(name);
// 	if (it != user._banded_channels.end()) {
// 			return true;
// 	}
//     return false;
// }

static bool if_target_opr(client_t &clients, std::string &target, std::string &name) {
    client_t::iterator it = clients.begin();
    for(; it  != clients.end(); ++it) {
        if (it->second.get_nickname() == target)
        {
            std::set<std::string>::iterator ite = it->second._owned_channels.find(name);
            if ( ite != it->second._owned_channels.end()) {  
                (*it).second._owned_channels.erase(ite);
                return true;
            }
        }
    }
    return false;
    
}

static bool banded(client_t &clients, channel_t &channels, std::string &name, std::string &target) {
    client_t::iterator it = clients.begin();
    for(; it  != clients.end(); ++it) {
        if (it->second.get_nickname() == target)
        {
            it->second._banded_channels.insert(name);
            std::set<std::string>::iterator ite = it->second._owned_channels.find(name);
            if ( ite != it->second._owned_channels.end()) {  
                (*it).second._owned_channels.erase(ite);
            }
        }
    }
    channel_t::iterator ite = channels.begin();
    for(; ite  != channels.end(); ++ite) {
        if (ite->first.first == name)
        {
            std::set<User>::iterator i = ite->second.begin();
            for (; i != ite->second.end(); ++i) {
                if (i->get_nickname() == target)
                    ite->second.erase(i);
            }
        }
    }
    return true;
}

static int	if_or_not(client_t &clients, channel_t &channels, std::string &name, std::string &target) {
	int result = 0;

	std::set<User>::iterator it;
    channel_t::iterator map;
	
	for (channel_t::iterator iter = channels.begin(); iter != channels.end(); ++iter) {
		std::pair<std::string, std::string> key = iter->first;
		if (key.first == name) {
            map = iter;
			it = (*iter).second.begin();
			for(; it != (*iter).second.end(); ++it) {
				if ((*it).get_nickname() == target) {
					std::set<std::string>::iterator i = (*it)._owned_channels.find(name);
					if ( i != (*it)._owned_channels.end()) {
						return 3;
						
					} if (i == (*it)._owned_channels.end() && !result) {
						result = 2;
                    	break ;
					}
				}
				if (result == 2)
					break;
			}
            if (!result) {
                result = 1;
			    break;
            }
		}
        if (result == 1)
            break;
	}
    if (result == 2) {
        client_t::iterator iti = clients.begin();
        for (; iti != clients.end(); ++iti)
            if ((*it).get_nickname() == (*iti).second.get_nickname()) {
                (*iti).second._owned_channels.insert(name);
                break;   
            }
    }
    if (result == 1) {
        client_t::iterator iti = clients.begin();
        for (; iti != clients.end(); ++iti)
            if (target == (*iti).second.get_nickname()) {
                (*iti).second._owned_channels.insert(name);
                break;   
            }
        (*map).second.insert((*iti).second);
        std::set<User>::iterator	ite = (*map).second.begin();
        for (; ite != (*map).second.end(); ite++)
        {
            std::string reply = msg_format((*iti).second) + " JOIN #" + name + "\r\n";
            send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
        }
    }
	return result;
	
}

void	omode(client_t &clients, size_t i, channel_t &channels, std::string &msg, std::string &name) 
{
	std::string target = trimFront(msg, 0);
    
	if (is_there(clients[i].second, name) && if_target(clients, target)) {
	    int r = if_or_not(clients, channels, name, target);
		if (r == 3) {
			// inform the client that the target already has the operater privileges ont the channel;
			std::cout << "the target is already +o" << std::endl;
		}
		if (r == 2) {
			// inform the client that the target gain operator privileges;
			std::cout << "now the target is +o" << std::endl;
		}
		if (r == 1) {
			// add the target to channel with the operater privileges;
			std::cout << "the target has joined the channel with +o" << std::endl;
		}
		
	} else {
		// inform the client that he has not the operater privileges on the channel;
            std::cout << "you are not operator on this channel or the target not valid user or not valid channel" << std::endl;
	}
	return;
}

void	o_mode(client_t &clients, size_t i, channel_t &channels, std::string &msg, std::string &name) 
{
	(void)channels;
	std::string target = trimFront(msg, 0);
    if (is_there(clients[i].second, name))
    {
        if(if_target_opr(clients, target, name)) {
            std::cout << "the target is just a regular user" << std::endl;
        } else {
            std::cout << "the target is not oper or is not on channel" << std::endl;
        }
    } else {
        std::cout << "You are not allowed to change on this channel" << std::endl;
    }
	return;
}

void	b_mode(client_t &clients, size_t i, channel_t &channels, std::string &msg, std::string &name) {
    (void)channels;
	std::string target = trimFront(msg, 0);
    if (is_there(clients[i].second, name))
    {
        if(if_target(clients, target)) {
            if (banded(clients, channels, name, target))
                std::cout << "the target is baned from this channel" << std::endl;
        } else {
            std::cout << "the target is not actif user on the server" << std::endl;
        }
    } else {
        std::cout << "You are not allowed to change on this channel" << std::endl;
    }
	return;
}

