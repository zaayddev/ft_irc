/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respons.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 23:46:49 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/31 05:56:41 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

std::string prompte()
{
    std::stringstream   ss;
    ss << BLUE << "[" << getTime() << "] " << RESET;
    return (ss.str());
}

void	welcome_msg(User user)
{
	std::string s;

	s = "U : " + user.get_nickname() \
		+ "!" + user.get_user() + "@" + user.get_ip() + "\r\n" + prompte();
    send(user.get_fd(), s.c_str(), s.length(), 0);
}

std::string	reject_msg(std::string cmd, int ern)
{
	std::stringstream	ss;
	
                    //PASS ERR
	if ( ern == 461 )
		ss << RED << "461 " << RESET << cmd << " :Not enough parameters\r\n";
    else if (ern == 462)
        ss << RED << "462 " << RESET << " :You may not reregister\r\n";
                    
                    //NICK ERR
    else if (ern == 431)
		ss << RED << "431" << RESET << " :No nickname given\r\n";
    else if (ern == 432)
		ss << RED << "432 " << RESET << cmd <<  " :Erroneus nickname\r\n";
    
    else if (ern == 464)
		ss << RED << "464" << RESET << " :Password incorrect\r\n";
	else if (ern == 464)
		ss << RED << "464" << RESET << " :Password incorrect\r\n";

    
	else if (ern == 1)
		ss << "Invalid character\r\n";
	else if (ern == 2)
		ss << "You already knoen as " << cmd << "\r\n";
	else if (!ern)
		ss << "Check [" << cmd << "] already exist\r\n";
	else if (ern == -1)
		ss << "Add a parameter : " << "[" << cmd << "] <PARAMETRE>\r\n";
	return (ss.str());
}

void    senderr(std::string cmd, int fd,int ern) {
    std::string reject = reject_msg(cmd, ern) + prompte();
	send(fd, reject.c_str(), reject.length(), 0);
}
