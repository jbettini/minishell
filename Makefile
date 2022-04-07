# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/03 04:39:29 by jbettini          #+#    #+#              #
#    Updated: 2022/04/07 01:20:14 by jbettini         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -g
NAME	=	minishell
I_DIR	=	includes
SRC_DIR =	srcs
OBJ_DIR =	srcs/objs
#USER	=	ydanset
#USER	=	jbettini

OBJ_PFX = 	srcs/objs/main.o
# IF NEEDED at 42 on MacOS10 Install :
#  rm -rf $HOME/.brew && git clone --depth=1 https://github.com/Homebrew/brew $HOME/.brew && echo 'export PATH=$HOME/.brew/bin:$PATH' >> $HOME/.zshrc && source $HOME/.zshrc && brew update
#  brew install readline
#LIBS	=	-L/Users/$(USER)/.brew/opt/readline/lib -lreadline -Llibft -lft
#HEADERS	=	-I$(I_DIR) -I/Users/$(USER)/.brew/opt/readline/include

# @HOME on MacOS12 Monterey
LIBS	=	-L/usr/local/opt/readline/lib -lreadline -Llibft -lft
HEADERS	=	-I$(I_DIR) -I/usr/local/opt/readline/include

SRC	= 	main.c \
 		builtins.c \
 		env.c \
 		exec.c \
 		redir.c \
 		connect.c \
 		error.c \
 		expand_ev.c \
 		free.c \
 		get_cmds.c \
 		get_tokens.c \
 		parse.c \
 		strs.c \
 		utils.c	\
		utils2.c	\
 		signals.c \
 		ft_strtok.c	\
		tty.c	\
		env_utils.c	\
		check.c	\
		expand_utils.c	\
		connect_utils.c	\
		simple_cmd.c	\
		multiple_cmds.c	\
		norm.c

SRC_PFX =	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ	=	$(SRC:%.c=%.o)

OBJ_PFX = $(addprefix $(OBJ_DIR)/, $(OBJ))

all: $(NAME)

$(NAME): $(OBJ_PFX)
	make -C libft
	$(CC) -o $(NAME) $(OBJ_PFX) $(LIBS)

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

.PHONY: all run clean fclean re