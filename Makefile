#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ktasan <ktasan@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/15 18:38:29 by ktasan            #+#    #+#              #
#    Updated: 2025/11/15 18:38:32 by ktasan           ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME_S = server
NAME_C = client
CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME_S) $(NAME_C)

$(NAME_S): server.c
	$(CC) $(CFLAGS) server.c -o $(NAME_S)

$(NAME_C): client.c
	$(CC) $(CFLAGS) client.c -o $(NAME_C)

clean:
	rm -f $(NAME_S) $(NAME_C)

fclean: clean

re: fclean all
