/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 23:35:19 by yelgharo          #+#    #+#             */
/*   Updated: 2023/02/10 15:41:54 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "Ircserv.hpp"

class User {
	private:
		std::string					_ip;
		std::string					_nickname;
		std::string					_username;
		std::string					_realname;
		int							_fd;
		bool						_is_complete;
		bool						_authentification;
		
	public:

		std::string					msg;
        std::vector<std::string>	_owned_channels;
		
		User(void);
		User(std::string str);

		std::string					get_owned_channels( void ) const;
		std::string					get_nickname(void) const;
		std::string					get_username(void) const;
		std::string					get_realname(void) const;
		std::string					get_ip(void) const;
		bool						get_authentification(void) const;
		bool						get_is_complete(void) const;
		int							get_fd(void) const;
	
		void						set_authentification(const bool authentification);
		void						set_owned_channels(const std::string owned_channel);
		void						set_username(const std::string username);
		void						set_realname(const std::string realname);
		void						set_nickname(const std::string nickname);
		void						set_is_complete(const bool is_complete);
		void						set_ip(const std::string ip);
		void						set_fd(int fd);     
		
};

#endif