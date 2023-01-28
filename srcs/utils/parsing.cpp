#include "../../Includes/Ircserv.hpp"

void	cmds_parsing(client_type &clients, channel_type &channels, std::string &msg, int i, std::string password) {
    std::string tmp;
    (void) channels;
    (void) password;
    (void) clients;
    (void) i;
    std::cout << "called handle_cmds with msg: " +  msg << std::endl;
	for (std::string::size_type	pos = msg.find("\r\n"); pos != std::string::npos; pos = msg.find("\r\n"))
	{
		if (pos == 0)
			std::cout << YELLOW << "WARNING" << RESET <<std::endl;
        tmp = msg.substr(0, pos);
		msg.erase(0, pos + 2);
        if (clients[i].second.get_is_complete() == false) {
            if (tmp.find("PASS ") == 0)
                user_authentification(clients, tmp, password, i);
        }
	}
}
