/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/26 23:18:14 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Ircserv_HPP
# define Ircserv_HPP

/*<------> DEFINE Variables <------>*/
# define BACKLOG 4

/*<------> INCLUDE Headers <------>*/
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <poll.h>
# include <iostream>

/*<------> SET Functions Prototype <------>*/
int     tcp_server(int port);
void	loop_connections(int socket_fd, std::string password);

#endif