# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: ythollet <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/05/04 20:51:29 by ythollet     #+#   ##    ##    #+#        #
#    Updated: 2019/03/02 16:37:00 by dewalter    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

.PHONY: all clean fclean re

CFLAGS = -g -Wall -Wextra -Werror

NAME = 42sh

DIR_LIB = ./Libft/
DIR_INC = ./Include/
DIR_SRC = ./Sources/
DIR_OBJ = ./Objects/

SRCS =  shell/main.c \
        shell/tools.c \
        shell/ft_tools.c \
        shell/shell_tools.c \
        shell/shell_envsub.c \
        shell/shell_env_init.c \
        shell/shell_error.c \
        shell/shell_hrdc.c \
        shell/shell_check.c \
        shell/shell_clean.c \
        shell/split/shell_split.c \
        shell/split/shell_split_getargs.c \
 		shell/split/shell_split_tools.c \
 		shell/split/shell_split_stdout.c \
 		shell/split/shell_split_stdin.c \
 		shell/split/shell_split_stdtools.c \
        shell/process/shell_process.c \
        shell/process/shell_process_prepare.c \
        shell/process/shell_process_output.c \
        shell/process/shell_process_output_tools.c \
        shell/process/shell_process_input.c \
        shell/process/shell_process_tools.c \
        shell/process/shell_process_getpathcmd.c \
        shell/process/shell_process_exec.c \
        shell/process/shell_process_plomberie.c \
        shell/process/shell_process_pipe.c \
		builtins/builtin_cd.c \
		builtins/builtin_cd_tools.c \
        builtins/builtin_allenv.c \
        builtins/builtin_echo.c \
        builtins/builtin_envl.c \
        builtins/builtin_type.c \
        builtins/builtin_type_tools.c \
        builtins/builtin_alias.c \
		builtins/builtin_alias_tools.c \
        builtins/builtin_unalias.c \
        builtins/builtin_exit.c \
        builtins/builtin.c \
        editor/get_stdin.c \
		editor/prompt.c \
		editor/cursor_position.c \
		editor/backspace.c \
		editor/go_to_of_line.c \
		editor/move_cursor_left.c \
		editor/move_cursor_right.c \
		editor/move_cursor_up.c \
		editor/move_cursor_down.c \
		editor/move_word.c \
		editor/add_char.c \
		editor/paste_line.c \
		editor/term_init.c \
		editor/tools.c \
		editor/window_resize.c \
		editor/insert_del_line.c \
		editor/delete_from_cursor_to_end.c \
		editor/clear_window.c \
		editor/paste_clipboard.c \
		editor/tabulator/tabulator_check.c \
		editor/tabulator/tabulator_check_mode.c \
		editor/tabulator/tabulator_put_row.c \
		editor/tabulator/tabulator_recup_data.c \
		editor/tabulator/tabulator_recup_folder_files.c \
		editor/tabulator/term_tabulator.c \
		editor/tabulator/tabulator_put_new_cmd.c \
		editor/tabulator/tabulator_check_if_var.c \
		editor/tabulator/tabulator_tools.c \
		editor/history/term_hist_recup_cmd.c \
		editor/history/term_history.c \
		editor/history/term_history_incremental_search.c

INCLUDES_FILE = shell.h

OBJS_FOLDERS = builtins editor editor/tabulator editor/history shell \
                shell/split shell/process
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
	@gcc -o $@ -c $< -I $(DIR_INC) $(CFLAGS)

clean:
	@rm -rf $(DIR_OBJ)
	@make clean -C $(DIR_LIB)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(DIR_LIB)

re: fclean all
