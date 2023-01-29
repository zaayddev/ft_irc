/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 02:34:58 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/29 20:26:17 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

std::string totrim(std::string toTrim, int i) {
	std::string trimed = "";

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

	i = client_msg.length();
	if (!client_msg.find("NICK")) {
		if (i > 5 && isspace(client_msg[4])) {
			std::string nick = totrim(client_msg, 5);
			if (check_input(nick, clients, fd))
				user.set_nickname(nick);
		} else {
			std::string reject = reject_msg("NICK", -1);
			send(fd, reject.c_str(), reject.length(), 0);
		}
	} else if (!client_msg.find("USER")) {
		if (i > 5 && isspace(client_msg[4])) {
			std::string _user = totrim(client_msg, 5);
			if (check_input(_user, clients, fd)) 
				user.set_user(_user);
		} else {
			std::string reject = reject_msg("USER", -1);
			send(fd, reject.c_str(), reject.length(), 0);
		}
	}
	if (user.get_nickname().size() && user.get_user().size()) {
		user.set_is_complete(true);
		std::string	welcome = welcome_msg(user);
		send(fd, welcome.c_str(), welcome.length(), 0);
	}
}