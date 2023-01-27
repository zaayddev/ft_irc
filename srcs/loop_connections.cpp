#include "../Includes/Ircserv.hpp"

void	loop_connections(int socket_fd, std::string password)
{
	std::vector<client>		clients;
	pollfd					socket;
	User					server("");
	
	socket.fd = socket_fd;
	socket.events = POLLIN;
	socket.revents = 0;
	clients.push_back(client(socket, server));
    (void) password;
	while (true) {
		initialise_poll(clients, clients.size());
		accept_call(clients, socket_fd);
	}
}