# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/24 13:51:13 by rahmed            #+#    #+#              #
#    Updated: 2022/03/30 17:13:04 by ydanset          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -g
NAME	=	minishell
I_DIR	=	includes
SRC_DIR =	srcs
OBJ_DIR =	srcs/objs
USER	=	ydanset

# IF NEEDED at 42 on MacOS10 Install :
#  rm -rf $HOME/.brew && git clone --depth=1 https://github.com/Homebrew/brew $HOME/.brew && echo 'export PATH=$HOME/.brew/bin:$PATH' >> $HOME/.zshrc && source $HOME/.zshrc && brew update
#  brew install readline
LIBS	=	-L/Users/$(USER)/.brew/opt/readline/lib -lreadline -Llibft -lft
HEADERS	=	-I$(I_DIR) -I/Users/$(USER)/.brew/opt/readline/include

# @HOME on MacOS12 Monterey
#LIBS	=	-L/usr/local/opt/readline/lib -lreadline -Llibft -lft
#HEADERS	=	-I$(I_DIR) -I/usr/local/opt/readline/include

OBJ_PFX = 	srcs/objs/main.o

SRC	= 	main.c \
		builtins.c \
		echo.c \
		env.c \
		exec.c \
		minishell.c \
		redir.c \
		miniparser.c \
		connect.c \
		pipe.c \
		dev.c \
		error.c \
		expand_ev.c \
		free.c \
		get_cmds.c \
		get_tokens.c \
		parse.c \
		strs.c \
		utils.c	\
		signal_del.c \
		ft_strtok.c

SRC_PFX =	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ	=	$(SRC:%.c=%.o)

OBJ_PFX = $(addprefix $(OBJ_DIR)/, $(OBJ))

all: $(NAME)

$(NAME): libft $(OBJ_PFX)
	$(CC) -o $(NAME) $(OBJ_PFX) $(LIBS)

libft:
	make -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

clean:
	make clean -C libft
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C libft
	rm  -rf $(NAME)

re: fclean all

.PHONY: all run clean fclean re libft debug
