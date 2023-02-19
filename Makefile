# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 16:33:22 by yelgharo          #+#    #+#              #
#    Updated: 2023/02/13 15:46:09 by yelgharo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

LIB = Includes/Ircserv.hpp \
		Includes/User.hpp 

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC =	main.cpp \
		transfer.cpp \
			srcs/tcp_server.cpp \
			srcs/User.cpp \
			srcs/server_join.cpp \
			srcs/loop_connections.cpp \
				srcs/bonus/bot.cpp \
				srcs/utils/check.cpp \
				srcs/utils/join.cpp \
				srcs/utils/modes.cpp \
				srcs/utils/operations.cpp \
				srcs/utils/poll.cpp \
				srcs/utils/parsing.cpp \
				srcs/utils/response.cpp \
				srcs/utils/tools.cpp

OFILES = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OFILES) 
	@${CC} $^ -o $@

%.o: %.cpp $(LIB)
	@$(CC) $(FLAGS) -c $< -o $@

clean :
	@rm -rf ${OFILES}

fclean : clean
	@rm -rf ${NAME}

re : fclean all