/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/27 21:35:56 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Ircserv_HPP
# define Ircserv_HPP

/*<------> DEFINE Variables <------>*/
# define BACKLOG 1
# define TIMEOUT (60 * 1000)

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
#include "Client.hpp"

/*<------> SET Functions Prototype <------>*/
int     tcp_server(int port);
void	loop_connections(int socket_fd, std::string password);
void	initialise_poll(std::vector<client> &clients, int fd_size);

#endif