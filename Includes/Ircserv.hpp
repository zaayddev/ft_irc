/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/10 15:53:16 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

/*<------> INCLUDE Headers <------>*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream> 
#include <fcntl.h>
#include <vector>
#include <list>
#include <map>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include "User.hpp"

/*<------> Forward Declaration <------>*/
class User;

/*<------> Alternate names for our data types <------>*/
typedef std::pair<pollfd, User>												client;
typedef std::pair<std::pair<std::string, std::string>, std::vector<User> >	channel;
typedef std::vector<client>													client_t;
typedef std::map<std::pair<std::string, std::string>, std::vector<User> >	channel_t;

/*<------> DEFINE Variables <------>*/
# define npos			std::string::npos
# define SERVER			"irc.1337.ma"
# define TIMEOUT		(60 * 60 * 1000)
# define BUFFER_SIZE	500
# define END			"\r\n"
# define BACKLOG 		1

/*<------> Define colors  <------>*/
# define PURPLE_BOLD	"\033[1;35m"
# define YELLOW_BOLD	"\033[1;33m"
# define GREEN_BOLD		"\033[1;32m"
# define BLUE_BOLD		"\033[1;34m"
# define RED_BOLD		"\033[1;31m"
# define PURPLE			"\033[35m"
# define YELLOW			"\033[33m"
# define GREEN			"\033[32m"
# define BLUE			"\033[34m"
# define RED			"\033[31m"
# define RST			"\033[0m"

/*<------> Channels and there keys <------>*/
struct s_list {
	
		std::list<std::string>	_channel;
		std::list<std::string>	_key;
};

/*<------> SET Used Functions <------>*/
int				first_in_check(char* s2, std::string s3);
int				getPort(char* s);
int				tcp_server(int port);

bool			channel_operations(client_t &clients, channel_t &channels, std::string msg, int i);
bool			check_input(std::string s, client_t &clients, int &i, int index);
bool			check_user_exist(std::vector<User> users, std::string nick);
bool			ifexist(client_t &clients, int i, std::string s);
bool			Alreadythere(User &user, std::string name);
bool			validNick(std::string nick);

void			cmds_parsing(client_t &clients, channel_t &channels, std::string &msg, int i, std::string password);
void			user_authentification(client_t &clients, std::string input, std::string password, size_t i);
void			join_channels(client_t &clients, size_t i, channel_t &channels, std::string &msg);
void			server_join(std::vector<client> &clients, std::string client_msg, int i);
void			senderr(std::string cmd, int i, client_t &clients, int ern);
void			initialise_poll(std::vector<client> &clients, int fd_size);
void			accept_call(std::vector<client> &clients, int socket_fd);
void			loop_connections(int socket_fd, std::string password);
void			bot(client_t &clients, size_t i, std::string &name);
void			close_connection(client_t &clients, size_t i);
void			nickchange_msg(User user, std::string str);
void			tolowstr(std::string &s);
void			welcome_msg(User user);

std::string		rcv_msg(int client_fd, std::vector<client> &clients, size_t i, channel_t &channels);
std::string		reject_msg(std::string cmd, int ern, client_t &clients, int i);
std::string		take_nickname_from_msg(std::string msg);
std::string		trimFront(std::string s, int i);
std::string		trim(std::string s, int i);
std::string		ip_itostr(in_addr_t ip);
std::string		msg_format(User &user);

#endif
