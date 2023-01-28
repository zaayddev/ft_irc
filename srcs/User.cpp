/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 00:00:18 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/28 09:51:15 by zchbani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/User.hpp"

User::User(void) { }
User::User(std::string str) : _nick(str) { }

std::string User::get_ip(void) const {
    return (_ip);
}
int User::get_fd(void) const {
    return (_fd);
}

void    User::set_ip(const std::string ip) {
    _ip = ip;
}
void    User::set_fd(int fd) {
    _fd = fd;
}