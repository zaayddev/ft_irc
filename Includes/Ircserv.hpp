/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/28 02:46:56 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Ircserv_HPP
# define Ircserv_HPP

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
typedef std::pair<pollfd, User>     client;

/*<------> DEFINE Variables <------>*/
# define BACKLOG 1
# define TIMEOUT (60 * 1000)


/*<------> SET Functions Prototype <------>*/
int     tcp_server(int port);
void	loop_connections(int socket_fd, std::string password);
void	initialise_poll(std::vector<client> &clients, int fd_size);
void    accept_call(std::vector<client> &clients, int socket_fd);

#endif