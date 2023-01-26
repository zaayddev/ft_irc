/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:34:23 by zchbani           #+#    #+#             */
/*   Updated: 2023/01/26 23:24:49 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

int tcp_server(int port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        std::cerr << "ERROR, socket() failed!!" << std::endl;

    int optval = 1;
    int reuse = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
    if (reuse == -1)
        std::cerr << "ERROR, setsockopt() failed!!" << std::endl;

    reuse = fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	if (reuse < 0)
		std::cerr << "ERROR, fcntl() failed!!" << std::endl;

    struct sockaddr_in addr;

	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	reuse = bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
	if (reuse == -1)
        std::cerr << "ERROR, bind() failed!!" << std::endl;

	reuse = listen(socket_fd, BACKLOG);
	if (reuse == -1)
        std::cerr << "ERROR, listen() failed!!" << std::endl;
	return (socket_fd);
}