#include "../../Includes/Ircserv.hpp"

static bool	check_password(std::string input, std::string pwd)
{
	if (input.substr(5, input.size() - 5) == pwd)
		return (true);
	else
		return (false);
}

void    user_authentification(client_type &clients, std::string input, std::string password, size_t i) {
    if (check_password(input, password) == true)
        clients[i].second.set_authentification(true);
    else
        std::cout << "Password invalide" << std::endl;
}

void	close_connection(client_type &clients, size_t i)
{
	if (close(clients[i].first.fd) == -1)
		std::cerr << "failed to close file descriptor" << std::endl;
	clients.erase(clients.begin() + i);
}

bool	check_nickname(std::string nick, client_type &clients)
{
	for (client_type::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (nick == it->second.get_nickname())
			return (true);
	}
	return (false);
}

bool	check_name(std::string input)
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		if (isalnum(input[i]) == 0)
		{
			std::cout << "invalid chars!" << std::endl;
			return (false);
		}
	}
	return (true);
}