/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 01:48:48 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/30 03:55:24 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

std::vector<pollfd> clients_fd(std::vector<client> &clients)
{
    std::vector<pollfd> clients_fd;

    for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        clients_fd.push_back((*it).first);
    }
    return (clients_fd);
}

void    accept_call(std::vector<client> &clients, int socket_fd)
{
    struct sockaddr_in  client_addr;
    socklen_t           lenght;

    while (true)
    {
        int new_fd = accept(socket_fd, (sockaddr*)&client_addr, &lenght);
        if (new_fd == -1)
        {
            if (errno != EWOULDBLOCK)
                std::cerr << "accept() failed" << std::endl;
            break;
        }
        pollfd	new_pollfd;
        User	new_user;
        new_pollfd.fd = new_fd;
        new_pollfd.events = POLLIN;
        new_pollfd.revents = 0;
        new_user.set_ip(ip_itostr(client_addr.sin_addr.s_addr));
        new_user.set_fd(new_fd);
        clients.push_back(client(new_pollfd, new_user));
        std::cout << YELLOW << "client " << RESET << "[ " << ip_itostr(client_addr.sin_addr.s_addr) \
            << " ]" << YELLOW << " is accepted" << std::endl;
        std::string p = prompte();
        send(new_user.get_fd(), p.c_str(), p.length(), 0);
    }
}

void	initialise_poll(std::vector<client> &clients, int fd_size)
{
    std::vector<pollfd> poll_fd = clients_fd(clients);
    std::cout << GREEN_BOLD << "poll() is waiting ..." << RESET << std::endl;
    int error = poll(poll_fd.data(), fd_size, TIMEOUT);
    if (error < 0)
        std::cout << RED_BOLD << "poll() is failed!!" << RESET << std::endl;
    else if (error == 0)
        std::cout << BLUE << "poll() in time out" << RESET << std::endl;
    for (size_t i = 0; i < clients.size(); i++)
        clients.begin()[i].first.revents = poll_fd[i].revents;
}

std::string	rcv_msg(int client_fd, std::vector<client> &clients, size_t i, channel_type &channels)
{
    char    buffer[BUFFER_SIZE];
    int     recv_data;

    (void) channels;

    // std::cout << "receive_msg called" << std::endl;
    std::memset(buffer, 0, sizeof(buffer));
    recv_data = recv(client_fd, buffer, sizeof(buffer), 0);
    if (recv_data) {
        std::string p = prompte();
        send(client_fd, p.c_str(), p.length(), 0);
    }
    
    //std::cout << buffer << std::endl;
    // [ EWOULDBLOCK ]the error message is not logged because it is expected to happen when the function is called in non-blocking mode. 
    // This can be used to prevent the program from getting stuck 
    // in an infinite loop waiting for data to be received. Instead, 
    // the function can continue execution and check for new data at a later time.
    if (recv_data < 0 && errno != EWOULDBLOCK)
        std::cout << "recv() failed" << std::endl;
    // IMPORTANT to note that recv() returning 0 is not an error, 
    // it is a normal condition, it is just indicating the end of the connection.
    if (recv_data == 0)
    {
        //kick_users_from_channels();
        close_connection(clients, i);
    }
    return (buffer);
}