/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 08:05:11 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/21 20:41:43 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Ircserv.hpp"

void	create_env_file(std::string name)
{
	std::ofstream	ofs(".env");
	std::string	var1 = "CLIENT_ID = \"cfde0e10b10247f194386e2f5d2d375f\"";
	std::string	var2 = "CLIENT_SECRET = \"6256e3fbb10e417eb5d7f55be82b81bc\"";
	std::string	var3 = "ARTIST_NAME = \"" + name + "\"";

	ofs << var1 << std::endl << var2 << std::endl << var3 << std::endl;
	ofs.close();
}

void	bot(client_t &clients, size_t i, std::string &name)
{
        create_env_file(trim(name, 6));
		std::string line;
		system("python ./srcs/bonus/bot.py");
        std::ifstream   ifs("songs");
        while (getline(ifs, line)) {
			std::string msg = ":ARTIST NOTICE " + clients[i].second.get_nickname() + " :♪ " + line + "\r\n";
   	 	    send(clients[i].second.get_fd(), msg.c_str(), msg.length(), 0);
		}
		ifs.close();
		system("rm -f songs .env");
}