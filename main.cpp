/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:24:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/03 10:33:52 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Ircserv.hpp"

int main(int argc, char **argv) {	
	int	port;
	int socket_fd;

	if (argc == 3) {
		if (!(port = first_in_check(argv[1], argv[2])))
				return (1);
        socket_fd = tcp_server(port);
        loop_connections(socket_fd, argv[2]);
    }
	else
		return (std::cerr << RED_BOLD << " ERROR;\n" << RED \
			<< "\t:./ircserv <port> <password>" << RST << std::endl, 1);
	return (0);
}


/*
	PING 
	PRIVMSG #
	PRIVMSG 
	JOIN #
	PART 
	OPER 
	KILL
	DIE
*/
// TO DO: send JOIN server responses [ 403 | 401 ]
// TO DO: create PRIVMSG # (private msgs for servers)
