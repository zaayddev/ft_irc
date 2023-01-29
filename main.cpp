/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:24:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/29 02:38:08 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Ircserv.hpp"

std::string getPassword(char *s) {
    return std::string(s);
}

int getPort(char *s) {
    if (strlen(s) > 5)
        return 0;
    for (int i = 0; s[i]; i++) {
        if(!isdigit(s[i]))
            return (0);
    }
    return (atoi(s));
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << RED << "./ircserv <port> <password>" << RED_BOLD << " expected format!" << RESET << std::endl;
        return (1);
    }
    int PORT = getPort(argv[1]);
    int socket_fd = tcp_server(PORT);
    if (PORT < 1 || PORT > 65535) {
        std::cerr << RED_BOLD << "error; Invalid port number" << RESET << std::endl;
        return (1);
    }
    if(!strlen(argv[2])) {
        std::cerr << RED_BOLD << "error; Invalid password" << RESET << std::endl;
        return (1);
    }
    std::string PASSWORD = getPassword(argv[2]);
    // std::cout << BLUE_BOLD << "server PASSWORD is : " << RESET << BLUE << PASSWORD << RESET << std::endl;
    // std::cout << BLUE_BOLD << "server PORT is     : " << RESET << BLUE << PORT     << RESET << std::endl;
    loop_connections(socket_fd, PASSWORD);
    return (0);
}