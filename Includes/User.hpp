/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 23:35:19 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/28 02:45:42 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef User_HPP
# define User_HPP

# include "Ircserv.hpp"

class User {
    private:
        std::string     _ip;
        std::string     _nick;
        std::string     _user;
        int             _fd;
        bool            _auth;
        
    public:
    
        User();
        User(std::string str);
        ~User();

        std::string get_ip(void) const;
        int         get_fd(void) const;

        void        set_ip(const std::string ip);
        void        set_fd(int fd);     
};

#endif