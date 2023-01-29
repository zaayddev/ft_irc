/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 23:35:19 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/29 05:50:28 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "Ircserv.hpp"

class User {
    private:
        std::string     _ip;
        std::string     _nickname;
        std::string     _user;
        int             _fd;
        bool            _is_complete;
        bool            _authentification;
        
    public:
    
        User(void);
        User(std::string str);
        std::string     msg;

        std::string get_ip(void) const;
        std::string	get_nickname(void) const;
        std::string get_user(void) const;
        bool	    get_authentification(void) const;
        bool        get_is_complete(void) const;
        int         get_fd(void) const;

        void        set_ip(const std::string ip);
        void		set_user(const std::string user);
        void        set_nickname(const std::string nickname);
        void        set_authentification(const bool authentification);
        void        set_is_complete(const bool is_complete);
        void        set_fd(int fd);     
};

#endif