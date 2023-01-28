/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/28 09:40:22 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

/*<------> INCLUDE Headers <------>*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <iostream> 
#include <fcntl.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <cstdlib>
#include "User.hpp"

class User;

/*<------> Alternate names for our data types <------>*/
typedef std::pair<pollfd, User>                         client;
typedef std::map< std::string, std::vector<User> >		channel_type;

/*<------> DEFINE Variables <------>*/
# define BACKLOG 1
# define TIMEOUT (3 * 60 * 1000)


/*<------> SET Functions Prototype <------>*/
int             tcp_server(int port);
void	        loop_connections(int socket_fd, std::string password);
void	        initialise_poll(std::vector<client> &clients, int fd_size);
void            accept_call(std::vector<client> &clients, int socket_fd);
std::string	    rcv_msg(int client_fd, std::vector<client> &clients, size_t i, channel_type &channels);

#endif