# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 16:33:22 by yelgharo          #+#    #+#              #
#    Updated: 2023/01/31 03:04:54 by yelgharo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

LIB = Includes/Ircserv.hpp Includes/Users.hpp 

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = 	main.cpp \
	srcs/tcp_server.cpp \
	srcs/loop_connections.cpp \
	srcs/utils/poll.cpp \
	srcs/utils/parsing.cpp \
	srcs/utils/tools.cpp \
	srcs/utils/respons.cpp \
	srcs/utils/operations.cpp \
	srcs/User.cpp \
	srcs/server_join.cpp

OFILES = $(SRC:.cpp=.o)

all : $(NAME) 

$(NAME) : $(OFILES)
	@${CC} $^ -o $@

%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

clean :
	@rm -rf ${OFILES}

fclean : clean
	@rm -rf ${NAME}

re : fclean all