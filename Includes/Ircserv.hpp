/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/31 06:50:59 by yelgharo         ###   ########.fr       */
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
typedef std::vector<client>                             client_t;
typedef std::map< std::string, std::vector<User> >		channel_t;

/*<------> DEFINE Variables <------>*/
# define BACKLOG 1
# define SERVER	"irc.1337.ma"
# define TIMEOUT (60 * 1000)
# define BUFFER_SIZE 500
# define END "\r\n"
# define npos std::string::npos


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
void			user_authentification(client_t &clients, std::string input, std::string password, size_t i);
void			loop_connections(int socket_fd, std::string password);
void			initialise_poll(std::vector<client> &clients, int fd_size);
void			accept_call(std::vector<client> &clients, int socket_fd);
std::string		rcv_msg(int client_fd, std::vector<client> &clients, size_t i, channel_t &channels);
void			cmds_parsing(client_t &clients, channel_t &channels, std::string &msg, int i, std::string password);
void			close_connection(client_t &clients, size_t i);
void			server_join(std::vector<client> &clients, std::string client_msg, int i);
bool			check_name(std::string input);
void    		welcome_msg(User user);
std::string		reject_msg(std::string cmd, int ern);
std::string		trim(std::string s, int i);
std::string		trimFront(std::string s, int i);
std::string     trimBack(std::string s);
std::string		getTime();
std::string		ip_itostr(in_addr_t ip);
std::string		prompte();
int				getPort(std::string s);
void            senderr(std::string cmd, int fd,int ern);
bool            channel_operations(client_t &clients, channel_t &channels, std::string msg, int i);
bool            validNick(std::string nick);

#endif
