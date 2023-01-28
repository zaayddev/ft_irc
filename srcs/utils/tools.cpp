#include "../../Includes/Ircserv.hpp"

void	close_connection(client_type &clients, size_t i)
{
	if (close(clients[i].first.fd) == -1)
		std::cerr << "failed to close file descriptor" << std::endl;
	clients.erase(clients.begin() + i);
}
