#include "../../Includes/Ircserv.hpp"

void	cmds_parsing(client_type &clients, channel_type &channels, std::string &msg, int i, std::string password) {
    std::cout << "called handle_cmds with msg: " +  msg << std::endl;
    (void) channels;
    (void) clients;
    (void) password;
    (void) i;
}
