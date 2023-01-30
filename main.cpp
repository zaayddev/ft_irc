/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:24:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/30 05:22:37 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Ircserv.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << RED << "./ircserv <port> <password>" << RED_BOLD << " expected format!" << RESET << std::endl;
        return (1);
    }
    int PORT = getPort(argv[1]);
    int socket_fd = tcp_server(PORT);
    if (PORT < 1 || PORT > 65535)
    {
        std::cerr << RED_BOLD << "error; Invalid port number" << RESET << std::endl;
        return (1);
    }
    if(!strlen(argv[2]))
    {
        std::cerr << RED_BOLD << "error; Invalid password" << RESET << std::endl;
        return (1);
    }
    std::string PASSWORD = getPassword(argv[2]);
    system("clear");
    loop_connections(socket_fd, PASSWORD);
    return (0);
}
// to do :
    // nick!user@ip                     -------> done 
    // if user < 5 && > 3 print error   -------> done
    // QUIT Command
    // Fix all the errors for invalid inputs for each command enterd 
    
