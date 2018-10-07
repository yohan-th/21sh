# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: ythollet <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/05/04 20:51:29 by ythollet     #+#   ##    ##    #+#        #
#    Updated: 2018/06/05 18:47:21 by ythollet    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all clean fclean re

CFLAGS = -g #-Wall -Wextra -Werror

NAME = 21sh

DIR_LIB = Libft
DIR_INC = Include
DIR_SRC = Sources
DIR_OBJ = Objects

SRCS =  main.c \
        get_path.c \
        tools.c \
        builtin_cd.c \
        builtin_allenv.c \
        builtin_echo.c \
        shell_split.c \
        shell_split_getargs.c \
        shell_split_envsub.c \
		shell_split_tools.c \
        builtin.c \
        shell_error.c \
        shell_process.c

INCLUDES_FILE = shell.h

OBJS = $(addprefix $(DIR_OBJ)/,$(SRCS:.c=.o))

all: lib $(NAME)

lib: 
		@if !(make -q -C $(DIR_LIB)); then \
			rm -f $(OBJ); \
			rm -f $(NAME); \
			make -C $(DIR_LIB);\
		fi;

$(NAME): $(OBJS)
	@make -C $(DIR_LIB)
	@gcc -o $(NAME) $(OBJS) -L $(DIR_LIB) -lft

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c  $(DIR_INC)/$(INCLUDES_FILE)
	@mkdir -p $(DIR_OBJ)
	gcc -o $@ -c $< -I $(DIR_INC) $(CFLAGS)

clean:
	@rm -rf $(DIR_OBJ)
	@make clean -C $(DIR_LIB)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(DIR_LIB)

re: fclean all
