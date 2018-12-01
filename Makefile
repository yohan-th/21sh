# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: ythollet <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/05/04 20:51:29 by ythollet     #+#   ##    ##    #+#        #
#    Updated: 2018/11/30 12:08:41 by dewalter    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all clean fclean re

CFLAGS = -g -Wall -Wextra -Werror

NAME = 21sh

DIR_LIB = ./Libft/
DIR_INC = ./Include/
DIR_SRC = ./Sources/
DIR_OBJ = ./Objects/

SRCS =  shell/main.c \
        shell/get_path.c \
        shell/tools.c \
        shell/shell_tools.c \
        shell/shell_split.c \
        shell/shell_split_getargs.c \
        shell/shell_split_envsub.c \
		shell/shell_split_tools.c \
		shell/shell_split_stdout.c \
		shell/shell_split_stdtools.c \
		shell/shell_error.c \
        shell/shell_process.c \
        shell/shell_split_heredoc.c \
		builtins/builtin_cd.c \
        builtins/builtin_allenv.c \
        builtins/builtin_echo.c \
        builtins/builtin.c \
        editor/get_stdin.c \
		editor/prompt.c \
		editor/cursor_position.c \
		editor/backspace.c \
		editor/go_to_of_line.c \
		editor/move.c \
		editor/move_cursor.c \
		editor/move_word.c \
		editor/add_char.c \
		editor/paste_line.c \
		editor/term_init.c \
		editor/find_env_var.c \
		editor/prompt_git.c \
		editor/move_to_previous_new_line.c \
		editor/window_resize.c \
		editor/insert_del_line.c \
		editor/delete_from_cursor_to_end.c \
		editor/clear_window.c \
		editor/end_of_text.c \
		editor/paste_clipboard.c \
		editor/term_history.c \
		editor/tabulator/tabulator_check.c \
		editor/tabulator/tabulator_check_mode.c \
		editor/tabulator/tabulator_put_row.c \
		editor/tabulator/tabulator_recup_data.c \
		editor/tabulator/tabulator_recup_folder_files.c \
		editor/tabulator/term_tabulator.c

INCLUDES_FILE = shell.h

OBJS_FOLDERS = builtins editor editor/tabulator shell
OBJS = $(addprefix $(DIR_OBJ),$(SRCS:.c=.o))
OBJS_FOLDERS_BIS = $(addprefix $(DIR_OBJ),$(OBJS_FOLDERS))

all: lib $(NAME)

lib:
		@if !(make -q -C $(DIR_LIB)); then \
			rm -f $(OBJ); \
			rm -f $(NAME); \
			make -C $(DIR_LIB);\
		fi;

$(NAME): $(OBJS)
	@make -C $(DIR_LIB)
	@gcc -o $(NAME) $(OBJS) -L $(DIR_LIB) -lft -ltermcap

$(DIR_OBJ)%.o: $(DIR_SRC)%.c  $(DIR_INC)/$(INCLUDES_FILE)
	@mkdir -p $(DIR_OBJ) $(OBJS_FOLDERS_BIS)
	@gcc -o $@ -c $< -I $(DIR_INC) #$(CFLAGS)

clean:
	@rm -rf $(DIR_OBJ)
	@make clean -C $(DIR_LIB)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(DIR_LIB)

re: fclean all
