CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRCS_SERVER = src/server.c
SRCS_CLIENT = src/client.c

NAME_SERVER = server
NAME_CLIENT = client

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER): $(SRCS_SERVER)
	$(CC) $(CFLAGS) $(SRCS_SERVER) -o $(NAME_SERVER)

$(NAME_CLIENT): $(SRCS_CLIENT)
	$(CC) $(CFLAGS) $(SRCS_CLIENT) -o $(NAME_CLIENT)

clean:
	rm -f $(NAME_SERVER) $(NAME_CLIENT)

re: clean all

.PHONY: all clean re
