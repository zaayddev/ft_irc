/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 01:48:48 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/28 05:10:44 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

std::string ip_itostr(in_addr_t ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    return std::string(std::to_string(bytes[0])+"."+std::to_string(bytes[1])+"." \
        +std::to_string(bytes[2])+"."+std::to_string(bytes[3]));
}

std::vector<pollfd> clients_fd(std::vector<client> &clients) {
    std::vector<pollfd> clients_fd;

    for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++) {
        clients_fd.push_back((*it).first);
    }
    return (clients_fd);
}

void accept_call(std::vector<client> &clients, int socket_fd) {
    struct sockaddr_in  client_addr;
    socklen_t           lenght;

    std::cout << "accept_call is called" << std::endl;
    while (true) {
        int new_fd = accept(socket_fd, (sockaddr*)&client_addr, &lenght);
        if (new_fd == -1) {
            if (errno != EWOULDBLOCK)
                std::cerr << "accept() failed" << std::endl;
            break;
        }
        pollfd	new_pollfd;
        User	new_user;
        new_pollfd.fd = new_fd;
        new_pollfd.events = POLLIN;
        new_pollfd.revents = 0;
        std::cout << "client addr before ----> : " << client_addr.sin_addr.s_addr << std::endl;
        std::cout << "client addr after  ----> : " << ip_itostr(client_addr.sin_addr.s_addr) << std::endl;
        new_user.set_ip(ip_itostr(client_addr.sin_addr.s_addr));
        new_user.set_fd(new_fd);
        clients.push_back(client(new_pollfd, new_user));
        std::cout << "new user is accepted" << std::endl;
    }
}

void	initialise_poll(std::vector<client> &clients, int fd_size) {
    std::vector<pollfd> poll_fd = clients_fd(clients);
    std::cout << "poll() is waiting ..." << std::endl;
    int error = poll(poll_fd.data(), fd_size, TIMEOUT);
    if (error < 0)
        std::cout << "poll() is failed" << std::endl;
    else if (error == 0)
        std::cout << "poll() in time out" << std::endl;
    for (size_t i = 0; i < clients.size(); i++)
        clients.begin()[i].first.revents = poll_fd[i].revents;
}

std::string	rcv_msg(int client_fd, std::vector<client> &clients, size_t i, channel_type &channels) {
    char    buffer[500];
    int     recv_data;

    (void) clients;
    (void) i;
    (void) channels;

    std::cout << "receive_msg called" << std::endl;
    std::memset(buffer, 0, sizeof(buffer));
    recv_data = recv(client_fd, buffer, sizeof(buffer), 0);
    std::cout << buffer << std::endl;
    // [ EWOULDBLOCK ]the error message is not logged because it is expected to happen when the function is called in non-blocking mode. 
    // This can be used to prevent the program from getting stuck 
    // in an infinite loop waiting for data to be received. Instead, 
    // the function can continue execution and check for new data at a later time.
    if (recv_data < 0 && errno != EWOULDBLOCK)
        std::cout << "recv() failed" << std::endl;
    // IMPORTANT to note that recv() returning 0 is not an error, 
    // it is a normal condition, it is just indicating the end of the connection.
    if (recv_data == 0) {
        //kick_users_from_channels();
        //close_connection();
    }
    return (buffer);
}