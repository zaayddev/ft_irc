/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 23:35:19 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/28 11:39:09 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "Ircserv.hpp"

class User {
    private:
        std::string     _ip;
        std::string     _nick;
        std::string     _user;
        int             _fd;
        //bool            _auth;
        
    public:
    
        User(void);
        User(std::string str);
        std::string     msg;

        std::string get_ip(void) const;
        int         get_fd(void) const;

        void        set_ip(const std::string ip);
        void        set_fd(int fd);     
};

#endif