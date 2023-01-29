/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 02:34:58 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/29 05:55:14 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

std::string totrim(std::string toTrim) {
	std::string trimed = "";
	int i = 5;

	while (toTrim[i] && isspace(toTrim[i]))
		i++;
	while (toTrim[i] && !isspace(toTrim[i])) {
		trimed += toTrim[i];
		i++;
	}
	while (toTrim[i] && isspace(toTrim[i]))
		i++;
	if (toTrim[i] && !isspace(toTrim[i])) {
		trimed += ' ';
	}
	return (trimed);
}

void	server_join(User &user, std::vector<client> &clients, std::string client_msg, int fd)
{
    (void) fd;
	int		i;

	if (!client_msg.find("NICK")) {
		i = client_msg.length();
		if (i > 5) {
			std::string nick ;
			nick = totrim(client_msg);
			std::cout << "this is nick :"<< nick << std::endl;
			if (check_name(nick) == false) {
				std::cout << "forbidden character in nickname" << std::endl;
				// send server response [ invalid nick charact ]
			} else if (check_nickname(nick, clients) == true) {
				std::cout << "nickname already in use" << std::endl;
				// send server response [ nickname exist ]
			} else
				user.set_nickname(client_msg.substr(5, (client_msg.length() - 5)));
		} else {
			std::cout << "please enter a nickname" << std::endl;
				// send server response [ no nickname enterd ]
		}
	} else if (client_msg.find("USER ") == 0)
		user.set_user(client_msg.substr(5, client_msg.find(" ", 5) - 5));
	if (user.get_nickname().size() && user.get_user().size()) {
		user.set_is_complete(true);
		std::string	welcome = welcome_msg(user);
		send(fd, welcome.c_str(), welcome.length(), 0);
	}
}