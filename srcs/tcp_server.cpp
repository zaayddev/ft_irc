/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:34:23 by zchbani           #+#    #+#             */
/*   Updated: 2023/01/30 03:11:20 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

int tcp_server(int port)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        std::cerr << RED_BOLD << "error; socket() failed!!" << RESET << std::endl;

    int optval = 1;
    int reuse = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
    if (reuse == -1)
        std::cerr << RED_BOLD << "error; setsockopt() failed!!" << RESET << std::endl;

    reuse = fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	if (reuse < 0)
		std::cerr << RED_BOLD << "error; fcntl() failed!!" << RESET << std::endl;

    struct sockaddr_in addr;

	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	reuse = bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
	if (reuse == -1)
    {
        std::cerr << RED_BOLD << "error; bind() failed!!" << RESET << std::endl;
        exit(1);
    }
	reuse = listen(socket_fd, 0);
	if (reuse == -1)
        std::cerr << RED_BOLD << "error; listen() failed!!" << RESET << std::endl;
	return (socket_fd);
}