#include "../Includes/Ircserv.hpp"

void	loop_connections(int socket_fd, std::string password)
{
	pollfd  socket;
	socket.fd = socket_fd;
	socket.events = POLLIN;
	socket.revents = 0;
    (void) password;
	while (true) {
		//init_poll();
	}
}