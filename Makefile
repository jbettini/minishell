# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/24 13:51:13 by rahmed            #+#    #+#              #
#    Updated: 2022/03/29 13:06:02 by ydanset          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	libminishell.a

LIBFT_DIR	=	libft
LIBFT_LIB	=	libft.a
SRC_DIR		=	srcs
INC_DIR		=	includes

SRCS		=	${SRC_DIR}/main.c \
				${SRC_DIR}/builtins.c \
				${SRC_DIR}/echo.c \
				${SRC_DIR}/env.c \
				${SRC_DIR}/exec.c \
				${SRC_DIR}/minishell.c \
				${SRC_DIR}/redir.c \
				${SRC_DIR}/miniparser.c \
				${SRC_DIR}/connect.c \
				${SRC_DIR}/pipe.c \
				${SRC_DIR}/dev.c \
				${SRC_DIR}/error.c \
				${SRC_DIR}/expand_ev.c \
				${SRC_DIR}/free.c \
				${SRC_DIR}/get_cmds.c \
				${SRC_DIR}/get_tokens.c \
				${SRC_DIR}/parse.c \
				${SRC_DIR}/strs.c \
				${SRC_DIR}/utils.c	\
				${SRC_DIR}/signals.c \
				${SRC_DIR}/ft_strtok.c

#SRCSB		=

MSRC		= 	${SRC_DIR}/main.c

OBJS		=	${SRCS:.c=.o}
#OBJSB		=	${SRCSB:.c=.o}

CC			=	gcc
RM			=	rm -f
AR			=	ar rcs
CFLAGS		=	-Wall -Wextra -Werror -g
FSFLAGS		=	-fsanitize=address

# IF NEEDED at 42 on MacOS10 Install :
#  rm -rf $HOME/.brew && git clone --depth=1 https://github.com/Homebrew/brew $HOME/.brew && echo 'export PATH=$HOME/.brew/bin:$PATH' >> $HOME/.zshrc && source $HOME/.zshrc && brew update
#  brew install readline
HEADER	=	-I${INC_DIR} -I/Users/$(USER)/.brew/opt/readline/include
LDFLAGS	=	-L${SRC_DIR} -L/Users/$(USER)/.brew/opt/readline/lib

# @HOME on MacOS12 Monterey
#HEADER		=	-I${INC_DIR} -I/usr/local/opt/readline/include
#LDFLAGS	=	-L${SRC_DIR} -L/usr/local/opt/readline/lib

MSHELLFLAG	=	-lreadline

TARGET_SRV	=	minishell

TXT_RED		=	\033[1;31m
TXT_GREEN	=	\033[1;32m
TXT_YELLOW	=	\033[1;33m
TXT_BLUE	=	\033[1;34m
TXT_MAGENTA	=	\033[1;35m
TXT_CYAN	=	\033[1;36m
BCK_RED		=	\033[0;41m
BCK_GREEN	=	\033[0;42m
BCK_YELLOW	=	\033[0;43m
BCK_BLUE	=	\033[0;44m
BCK_MAGENTA	=	\033[0;45m
BCK_CYAN	=	\033[0;46m
FANCY_RESET	=	\033[0m

%.o			:	%.c
				@echo "${TXT_BLUE}"
				@echo "\n~~~~~~~ MAKE PROJECT ~~~~~~~~"
				${CC} -c ${CFLAGS} -o $@ $< ${HEADER}
				@echo "${FANCY_RESET}"

${NAME}		:	${OBJS}
				@echo "${TXT_GREEN}"
				@echo "\n~~~~~~~~ MAKE LIBFT ~~~~~~~~~"
				make all -C ${LIBFT_DIR}
				@echo "${TXT_CYAN}"
				@echo "\n~~~~~~~~  LINKING ~~~~~~~~~~~"
				cp ${LIBFT_DIR}/${LIBFT_LIB} ${NAME}
				@echo "${TXT_YELLOW}"
				${AR} ${NAME} ${OBJS}
				@echo "${TXT_YELLOW}"
				@echo "\n~~~~~~~ COMPILATION ~~~~~~~~~"
				@echo "${TXT_GREEN}"
#				${CC} ${CFLAGS} ${LDFLAGS} ${MSHELLFLAG} ${NAME} ${MSRC} -o ${TARGET_SRV}
				${CC} ${CFLAGS} ${LDFLAGS} ${MSHELLFLAG} ${FSFLAGS} ${NAME} ${MSRC} -o ${TARGET_SRV}
				@echo "${FANCY_RESET}"
				mv srcs/*.o srcs/Obj

all			:	${NAME}
#bonus 		:	${NAMEB}

clean		:
				@echo "${TXT_YELLOW}"
				@echo "\n~~~~~~~~~~ CLEAN ~~~~~~~~~~~~"
				@echo "${TXT_RED}"
				${RM} ${OBJS}
#				${RM} ${OBJSB}
				@echo "${TXT_GREEN}"
				make clean -C ${LIBFT_DIR}
				@echo "$(FANCY_RESET)"

fclean		:	clean
				@echo "${TXT_YELLOW}"
				@echo "\n~~~~~~~~~~ FCLEAN ~~~~~~~~~~"
				@echo "${TXT_MAGENTA}"
				${RM} ${NAME}
				@echo "${TXT_GREEN}"
				make fclean -C ${LIBFT_DIR}
				rm srcs/Obj/*.o
				@echo "$(FANCY_RESET)"

re			:	fclean all

.PHONY		:	all clean fclean re norm flush exec test
############### PARTIE TEST ###############
test		:
				make all
				@echo "${TXT_YELLOW}"
				@echo "\n~~~~~~~~~~   TEST MINISHELL   ~~~~~~~~~~~"
				@echo "${FANCY_RESET}"
				./${TARGET_SRV} ${ARG}

norm		:
				@echo "${TXT_YELLOW}"
				@echo "\n~~~~~~~ CHECK LIB NORM ~~~~~~~~~"
				@echo "${TXT_GREEN}"
				norminette ${LIBFT_DIR}/*.c -R CheckForbiddenSourceHeader
				norminette ${LIBFT_DIR}/*.h -R CheckDefine
				@echo "${TXT_YELLOW}"
				@echo "\n~~~~~ CHECK PROJECT NORM ~~~~~~~"
				@echo "${TXT_CYAN}"
				norminette ${SRC_DIR}/*.c -R CheckForbiddenSourceHeader
				norminette ${INC_DIR}/*.h -R CheckDefine
				@echo "${FANCY_RESET}"

flush		:	fclean
				@echo "${TXT_YELLOW}"
				@echo "\n~~~~~~~ Extra-CLEANING  ~~~~~~~~"
				@echo "${TXT_RED}"
				${RM} ${TARGET_SRV}
				${RM} .DS_STORE
				${RM} *.out
				${RM} *.a
				${RM} ${SRC_DIR}/*.o
				${RM} ${LIBFT_DIR}/*.a
				${RM} ${LIBFT_DIR}/*.o
				${RM} -R *.dSYM
				@echo "${FANCY_RESET}"
###########################################
