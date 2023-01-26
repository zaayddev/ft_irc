/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:24:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/26 22:26:28 by yelgharo         ###   ########.fr       */
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
        std::cerr << "./ircserv <port> <password> format expected" << std::endl;
        return (1);
    }
    //int socket_fd = tcp_server(PORT);
    int PORT = getPort(argv[1]);
    if (PORT < 1 || PORT > 65535) {
        std::cerr << "Invalid port number" << std::endl;
        return (1);
    }
    if(!strlen(argv[2])) {
        std::cerr << "Invalid password" << std::endl;
        return (1);
    }
    std::string PASSWORD = getPassword(argv[2]);
    std::cout << "server password is : " << PASSWORD << std::endl;
    std::cout << "server port is : " << PORT << std::endl;
    std::cout << "Server info done" << std::endl;
    // create a function to loop over requests
    return (1);
}