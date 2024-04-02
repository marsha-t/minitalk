SERVER_CFILES = server.c
SERVER_OFILES = ${SERVER_CFILES:.c=.o}
CLIENT_CFILES = client.c
CLIENT_OFILES = ${CLIENT_CFILES:.c=.o}

SERVER_BONUS_CFILES = server_bonus.c 
SERVER_BONUS_OFILES = ${SERVER_BONUS_CFILES:.c=.o}
CLIENT_BONUS_CFILES = client_bonus.c error_bonus.c
CLIENT_BONUS_OFILES = ${CLIENT_BONUS_CFILES:.c=.o}

LIBFT = libft.a
LIBFT_DIR = libft
CFLAGS = -Wall -Wextra -Werror

all: server client

%o: %c 
	cc ${CFLAGS} -c $< -o ${<:.c=.o}

server:	$(SERVER_OFILES) $(LIBFT)
	cc $(CFLAGS) $(SERVER_OFILES) $(LIBFT_DIR)/$(LIBFT) -o server

client: $(CLIENT_OFILES) $(LIBFT)
	cc $(CFLAGS) $(CLIENT_OFILES) $(LIBFT_DIR)/$(LIBFT) -o client

$(LIBFT):
	make -C $(LIBFT_DIR)

bonus: server_bonus client_bonus

server_bonus: $(SERVER_BONUS_OFILES) $(LIBFT)
	cc $(CFLAGS) $(SERVER_BONUS_OFILES) $(LIBFT_DIR)/$(LIBFT) -o server_bonus

client_bonus: $(CLIENT_BONUS_OFILES)
	cc $(CFLAGS) $(CLIENT_BONUS_OFILES) $(LIBFT_DIR)/$(LIBFT) -o client_bonus

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(SERVER_OFILES) $(CLIENT_OFILES) 
	rm -f $(SERVER_BONUS_OFILES) $(CLIENT_BONUS_OFILES)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f server client
	rm -f server_bonus client_bonus

re: fclean all

.PHONY: all clean fclean re server client bonus 
