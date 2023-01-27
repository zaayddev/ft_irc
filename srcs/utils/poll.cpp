#include "../../Includes/Ircserv.hpp"

std::vector<pollfd> clients_fd(std::vector<client> &clients) {
    std::vector<pollfd> clients_fd;

    for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++) {
        clients_fd.push_back((*it).first);
    }
    return (clients_fd);
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
