# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zchbani <zchbani@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 16:33:22 by yelgharo          #+#    #+#              #
#    Updated: 2023/01/27 21:20:21 by zchbani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = 	main.cpp \
		srcs/tcp_server.cpp \
		srcs/loop_connections.cpp \
		srcs/utils/poll.cpp \

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