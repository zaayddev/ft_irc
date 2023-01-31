/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:24:50 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/31 07:15:22 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Ircserv.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
        return (std::cerr << RED_BOLD << " ERROR;\n" << RED \
            << "\t:./ircserv <port> <password>" << RESET << std::endl, 1);
    int PORT = getPort(argv[1]);
    int socket_fd = tcp_server(PORT);
    if (PORT < 1 || PORT > 65535)
        return (std::cerr << RED_BOLD << "ERROR;\n\tInvalid port number" << \
        RESET << std::endl, 1);
    if(!strlen(argv[2]))
        return (std::cerr << RED_BOLD << "ERROR;\n\tInvalid password" << RESET \
            << std::endl, 1);
    //system("clear");
    loop_connections(socket_fd, argv[2]);
    return (0);
}
// to do :
    // nick!user@ip                     -------> done 
    // if user < 5 && > 3 print error   -------> done
    // QUIT Command
    // Fix all the errors for invalid inputs for each command enterd
    // change nickname need to finish;;
    
    
