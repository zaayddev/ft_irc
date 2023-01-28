/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 00:00:18 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/28 00:01:54 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/User.hpp"

User::User() {}

User::User(std::string str) { (void) str;}

User::~User() {}

std::string User::get_ip(void) const {
    return (_ip);
}

void    User::set_ip(const std::string ip) {
    _ip = ip;
}
void    User::set_fd(int fd) {
    _fd = fd;
}