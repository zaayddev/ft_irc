#include "functions.hpp"

int main(int argc, char **argv) {
    int PORT = std::atoi(argv[1]);
    std::string password = get_password(argv[2]);
    int socket_fd = tcp_server(PORT);

    if (argc != 3)
        std::cerr << "./ircserv <port> <password> format expected" << std::endl;
    if (PORT < 1 || PORT > 65535)
        std::cerr << "Invalid port number" << std::endl;
    std::cout << "Server info done" << std::endl;
    // create a function to loop over requests
    return (1);
}