#include "../../Includes/Ircserv.hpp"

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
        new_pollfd.fd = new_fd;
        new_pollfd.events = POLLIN;
        new_pollfd.revents = 0;
        User	new_user;
        new_user.set_ip(ip_itostr(client_addr.sin_addr.s_addr));
        // TO DO : create a function (ip_itostr) appears to take an IP address in binary form (in_addr_t) and converts it
        // to a string in the standard "dot-decimal" notation (e.g. "192.168.0.1").
        // It uses a bit shift and bitwise AND operation to extract each of the four octets of the IP address,
        // and then streams these octets
        // into a stringstream object separated by periods. The final string representation of the IP address is returned by the function.
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
