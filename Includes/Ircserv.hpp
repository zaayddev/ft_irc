/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 22:25:01 by yelgharo          #+#    #+#             */
/*   Updated: 2023/01/26 22:26:01 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Ircserv_HPP
# define Ircserv_HPP

/*<------> INCLUDE Headers <------>*/
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <iostream>

/*<------> SET Functions Prototype <------>*/
int tcp_server(int port);

#endif