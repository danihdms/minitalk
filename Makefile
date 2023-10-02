INC_D		=	./includes/

SRC_CLI		:=	./src/client.c
SRC_SRV		:=	./src/server.c

OBJ_CLI		=	$(SRC_CLI:%.c=%.o)
OBJ_SRV		=	$(SRC_SRV:%.c=%.o)

NAME_CLI	=	client
NAME_SRV	=	server


# compilation
CC			=	gcc
CFLAG		=	-Wall -Werror -Wextra -g

# for debug
#CFLAG		=	-Weverything

#dependencies library
INC_LIB		=	./libft/
LIBFLAG		=	-L./libft -lft


%.o: %.c
	@$(CC) $(CFLAG) -I$(INC_D) -I$(INC_LIB) -c $< -o $@ 
	@echo "***compilation of '$<' in '$@'***"

all: $(NAME_CLI) $(NAME_SRV)

libft:
	@git submodule update --remote
	@(cd libft && make)

$(NAME_CLI): libft $(OBJ_CLI) 
	@$(CC) $(CFLAG) -I$(INC_D) -I$(INC_LIB) $(OBJ_CLI) -o $(NAME_CLI) $(LIBFLAG)
	@echo "***compilation of client***"

$(NAME_SRV):libft $(OBJ_SRV)
	@$(CC) $(CFLAG) -I$(INC_D) -I$(INC_LIB) $(OBJ_SRV) -o $(NAME_SRV) $(LIBFLAG)
	@echo "***compilation of server***"

clean:
	@rm -f $(OBJ_CLI) $(OBJ_SRV)
	@(cd libft && make clean)
	@echo "***deletion of all objects files***"

fclean: clean
	@rm -f $(NAME_CLI) $(NAME_SRV)
	@(cd libft && make fclean)
	@echo "***deletion of binary $(NAME_CLI) and $(NAME_SRV)***"

re: fclean all

.PHONY: all clean fclean re libft
