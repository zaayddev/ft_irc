/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:34:23 by zchbani           #+#    #+#             */
/*   Updated: 2023/02/21 22:52:08 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

/*
 * This function creates a TCP server socket that listens for incoming connections on the specified port.
 * It returns the file descriptor for the listening socket on success, or -1 on failure.
 */
int tcp_server(int port, struct addrinfo **p)
{
    int socket_fd, reuse;

    /*
     * Set up a socket address structure to specify the IP address and port to bind to.
     * hints is either a null pointer or a pointer to an addrinfo structure that the caller fills in with hints
     * about the types of information the caller wants returned.
     */
    struct addrinfo hints, *result;

    std::memset(&hints, 0, sizeof hints); // zero out the hints structure
    hints.ai_family = AF_UNSPEC;          // IPv4 or IPv6 [AF_UNSPEC, AF_INET, AF_INET6]
    hints.ai_socktype = SOCK_STREAM;      // TCP socket
    hints.ai_flags = AI_PASSIVE;          // use the IP address of the local host
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

    /*
     * the AI_ADDRCONFIG flag is set in the hints.ai_flags field.
     * This causes getaddrinfo to only return addresses that are compatible with the system's network interfaces,
     * which should exclude any IPv6 addresses if IPv6 is not currently enabled on the system.
     */

    char port_str[6];
    std::snprintf(port_str, sizeof port_str, "%d", port);

    /*
     * Get a list of address structures for the local host on the specified port.
     * the function returns through the result pointer a pointer to a linked list of addrinfo structures
     */
    if ((reuse = getaddrinfo(nullptr, port_str, &hints, &result)) != 0)
    {
        std::cerr << RED_BOLD << "getaddrinfo error: " << gai_strerror(reuse) << RST << std::endl;
        return -1;
    }

    /*
     * Iterate over the list of address structures and bind the socket to the first address that works.
     * If binding is successful, break out of the loop and use the current address.

     * the pointer *p is being used to store the list of available addresses returned by getaddrinfo().
    */

    for (*p = result; *p != nullptr; *p = (*p)->ai_next)
    {
        if ((*p)->ai_family == AF_UNSPEC || (*p)->ai_family == AF_INET || (*p)->ai_family == AF_INET6)
        {
            if ((socket_fd = socket((*p)->ai_family, (*p)->ai_socktype, (*p)->ai_protocol)) == -1)
            {
                // If creating the socket fails, output an error message and try the next address.
                std::cerr << RED_BOLD << "socket() failed.." << RST << std::endl;
                continue;
            }
            
            int optval = 1;
            if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) == -1)
            {
                std::cerr << "setsockopt() failed" << std::endl;
                return -1;
            }

            // Set timeout on socket
            struct timeval timeout;
            timeout.tv_sec = 5; // 5 seconds
            timeout.tv_usec = 0;

            reuse = bind(socket_fd, (*p)->ai_addr, (*p)->ai_addrlen);
            if (reuse == -1)
            {
                // If binding to the address fails, close the socket and try the next address.
                std::cerr << RED_BOLD << strerror(errno) << ", " << "bind() failed.." << RST << std::endl;
                close(socket_fd);
                exit(1);
            }
            break;
        }

        /*
         * The output IP address "::" indicates an IPv6 unspecified address, also known as the "unspecified address" or "any address".
         * This address is used to indicate that the socket is willing to accept incoming connections from any source address. In other words,
         * the socket is bound to all available network interfaces on the system
         */
    }

    // Free the list of address structures returned by getaddrinfo.
    // freeaddrinfo(result);

    // if a successful bind did not occur, output an error message and return -1
    if (*p == nullptr)
    {
        std::cerr << RED_BOLD << "failed to bind" << RST << std::endl;
        return -1;
    }

    // start listening for incoming connections on the bound socket
    reuse = listen(socket_fd, 0);
    if (reuse == -1)
    {
        std::cerr << RED_BOLD << "listen() failed" << RST << std::endl;
        return -1;
    }

    // Set the socket to non-blocking mode.
    reuse = fcntl(socket_fd, F_SETFL, O_NONBLOCK);
    if (reuse < 0)
        std::cerr << RED_BOLD << "error; fcntl() failed!!" << RST << std::endl;

    return socket_fd;
}