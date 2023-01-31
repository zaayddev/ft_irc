#include "../../Includes/Ircserv.hpp"

void    ping_message(client_type &clients, size_t i)
{
	std::string reply = "PONG " + (std::string)SERVER + "\r\n";
	send(clients[i].second.get_fd(), reply.c_str(), reply.length(), 0);
}

bool    channel_operations(client_type &clients, channel_type &channels, std::string msg, int i)
{
	std::string	reply;
    (void) channels;
	std::cout << "channel_operations:: ";
	if (msg.find("PING") == 0)
		ping_message(clients, i);
	return (false);
}