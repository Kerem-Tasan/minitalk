/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*                                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktasan <ktasan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 02:03:16 by ktasan            #+#    #+#             */
/*   Updated: 2025/11/15 03:34:13 by ktasan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
