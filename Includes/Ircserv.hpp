/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/30 21:15:04 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

/*<------> INCLUDE Headers <------>*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <sstream>
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
typedef std::vector<client>                             client_type;
typedef std::map< std::string, std::vector<User> >		channel_type;

/*<------> DEFINE Variables <------>*/
# define BACKLOG 1
# define TIMEOUT (60 * 1000)
# define BUFFER_SIZE 500

/*<------> Define colors //that we won't gonna need <------>*/
# define RED			"\033[31m"
# define RED_BOLD		"\033[1;31m"
# define GREEN			"\033[32m"
# define GREEN_BOLD		"\033[1;32m"
# define BLUE			"\033[34m"
# define BLUE_BOLD		"\033[1;34m"
# define PURPLE			"\033[35m"
# define PURPLE_BOLD	"\033[1;35m"
# define YELLOW			"\033[33m"
# define YELLOW_BOLD	"\033[1;33m"
# define RESET			"\033[0m"

/*<------> SET Functions Prototype <------>*/
int				tcp_server(int port);
void			user_authentification(client_type &clients, std::string input, std::string password, size_t i);
void			loop_connections(int socket_fd, std::string password);
void			initialise_poll(std::vector<client> &clients, int fd_size);
void			accept_call(std::vector<client> &clients, int socket_fd);
std::string		rcv_msg(int client_fd, std::vector<client> &clients, size_t i, channel_type &channels);
void			cmds_parsing(client_type &clients, channel_type &channels, std::string &msg, int i, std::string password);
void			close_connection(client_type &clients, size_t i);
void			server_join(std::vector<client> &clients, std::string client_msg, int i);
bool			check_input(std::string nick, client_type &clients, int fd, int index);
bool			check_name(std::string input);
std::string		welcome_msg(User user);
std::string		reject_msg(std::string cmd, int i);
std::string		totrim(std::string toTrim, int i);
std::string		trimPass(std::string toTrim, int i);
std::string		getTime();
std::string		ip_itostr(in_addr_t ip);
std::string		prompte();
int				getPort(char *s);
std::string		getPassword(char *s);

#endif