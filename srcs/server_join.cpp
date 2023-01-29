/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 02:34:58 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/29 02:34:59 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Ircserv.hpp"

std::string totrim(std::string toTrim) {
	std::string Trimed = "";
	int i = 5;

	while (toTrim[i] && isspace(toTrim[i]))
		i++;
	while (toTrim[i] && !isspace(toTrim[i])) {
		Trimed += toTrim[i];
		i++;
	}
	while (toTrim[i] && isspace(toTrim[i]))
		i++;
	if (toTrim[i] && !isspace(toTrim[i])) {
		Trimed += ' ';
	}
	return (Trimed);
}

void	server_join(User &user, std::vector<client> &clients, std::string client_msg, int fd)
{
    (void) fd;
	int		i;

	std::cout << "server_join" << std::endl;
	if (!client_msg.find("NICK"))
	{
		i = client_msg.length();
		if (i > 5) {
			std::string nick ;
			nick = totrim(client_msg);
			std::cout << "this is nick :"<< nick << std::endl;
			if (check_name(nick) == false)
			{
				std::cout << "forbidden character in nickname" << std::endl;
				// send server response [ invalid nick charact ]
			}
			else if (check_nickname(nick, clients) == true)
			{
				std::cout << "nickname already in use" << std::endl;
				// send server response [ nickname exist ]
			}
			else
				user.set_nickname(client_msg.substr(5, (client_msg.length() - 5)));
		} else {
			std::cout << "please enter a nickname" << std::endl;
				// send server response [ no nickname enterd ]
		}
	}
}