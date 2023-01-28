#include "../Includes/Ircserv.hpp"

void	server_join(User &user, std::vector<client> &clients, std::string client_msg, int fd)
{
    (void) fd;
	std::cout << "server_join" << std::endl;
	if (client_msg.find("NICK ") == 0)
	{
		std::string nick = client_msg.substr(5, (client_msg.length() - 5));
		if (check_nickname(nick, clients) == true)
		{
			std::cout << "nickname already in use" << std::endl;
            // send server response [ nickname exist ]
		}
		else if (check_name(nick) == false)
		{
			std::cout << "forbidden character in nickname" << std::endl;
            // send server response [ invalid nick charact ]
		}
		else
			user.set_nickname(client_msg.substr(5, (client_msg.length() - 5)));
	}
}