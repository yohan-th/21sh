/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   editor.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dzonda <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/10 00:46:23 by dzonda       #+#   ##    ##    #+#       */
/*   Updated: 2018/12/18 22:43:09 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H
# include <stdio.h>
# include <stdlib.h>
# include <term.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <dirent.h>
# define WHITE "\033[7;49;37m"
# define RED "\033[1;31m"
# define BLUE "\033[1;36m"
# define YEL "\033[1;33m"
# define END "\033[0m"
# define LGRAY "\033[47m"
# define LEFT_KEY (!ft_strcmp("\E[D", ed->key))
# define RIGHT_KEY (!ft_strcmp("\E[C", ed->key))
# define UP_KEY (!ft_strcmp("\E[A", ed->key))
# define DOWN_KEY (!ft_strcmp("\E[B", ed->key))
# define TAB_KEY (!ft_strcmp("\t", ed->key))
# define ENTER_KEY (!ft_strcmp("\n", ed->key))
# define HOME_KEY (!ft_strcmp("\E[H", ed->key))
# define END_KEY (!ft_strcmp("\E[F", ed->key))
# define SHIFT_UP (!ft_strcmp("\E[1;2A", ed->key))
# define SHIFT_DOWN (!ft_strcmp("\E[1;2B", ed->key))
# define SHIFT_RIGHT (!ft_strcmp("\E[1;2C", ed->key))
# define SHIFT_LEFT (!ft_strcmp("\E[1;2D", ed->key))
# define CTRL_A (ed->key[0] == 1 && ed->key[1] == 0)
# define CTRL_E (ed->key[0] == 5 && ed->key[1] == 0)
# define CTRL_C (ed->key[0]== 3 && ed->key[1] == 0)
# define CTRL_D (ed->key[0] == 4 && ed->key[1] == 0)
# define CTRL_L (ed->key[0] == 12 && ed->key[1] == 0)
# define CTRL_K (ed->key[0] == 11 && ed->key[1] == 0)
# define CTRL_U (ed->key[0] == 21 && ed->key[1] == 0)
# define CTRL_W (ed->key[0] == 23 && ed->key[1] == 0)
# define CTRL_P (ed->key[0] == 16 && ed->key[1] == 0)
# define BACKSPACE (ed->key[0] == 127 && ed->key[1]  == 0)

typedef struct winsize	t_sz;
typedef struct dirent	t_dirent;
typedef struct stat		t_stat;
typedef struct termios t_term;
typedef  enum		s_shortcut
{
	CTRLD = -2,
	CTRLC = -3,
}					e_shortcut;

typedef enum		s_prompt
{
	PROMPT,
	B_QUOTE,
	S_QUOTE,
	D_QUOTE,
	HRDC,
}					e_prompt;

typedef struct		s_history
{
	char				*cmd;
	struct s_history	*next;
	struct s_history	*prev;
}					t_history;

typedef struct		s_tab_elem
{
	char				*d_name;
	unsigned long		d_namlen;
	unsigned long		d_type;
	struct s_tab_elem	*next;
	struct s_tab_elem	*prev;
}					t_tab_elem;

typedef struct		s_tab
{
	char			*path;
	char			*data;
	char			*comp;
	char			*home;
	char			**env;
	DIR				*dir;
	int				nb_col;
	int				nb_row;
	int				max_len;
	int				nb_node;
	int				start;
	int				end;
	int				mode;
	int				save_pos;
	t_tab_elem		*elem;
	t_tab_elem		*last_elem;
}					t_tab;

typedef struct		s_editor
{
	int				ret;
	int				cur_col;
	int				cur_row;
	int				ws_row;
	int				ws_col;
	int				first_row;
	int				last_row;
	int				first_char;
	int				last_char;
	int				cursor_str_pos;
	int				prompt_size;
	char			*clipboard;
	char			*key;
	struct			termios term_default;
	t_history		*hist;
}					t_editor;

/*
 *******************************************************************************
 **                              CURSOR_MOVEMENT                               *
 *******************************************************************************
 */

void	move_cursor_left(t_editor *ed);
void	move_cursor_right(t_editor *ed);
void	move_cursor_up(t_editor *ed);
void	move_cursor_down(t_editor *ed);
void	move_word_left(t_editor *ed);
void	move_word_right(t_editor *ed);
void	go_to_begin_of_line(t_editor *ed);
void	go_to_end_of_line(t_editor *ed);
int		backspace(t_editor *ed);

/*
 *******************************************************************************
 **                                     CTRL                                   *
 *******************************************************************************
 */

int		clear_window(t_editor *ed, e_prompt prompt);
void	myhandler_interrupt(int signal);

/*
 ** ****************************************************************************
 **                              SAVE_RESET_CURSOR_POS                         *
 ** ****************************************************************************
 */

char	*cursor_position_escape_sequence(int row, int col, t_editor *ed);
void	reset_cursor_position_escape_sequence(char **cursor_position);

void	add_paste_into_line(t_editor *ed);
char	*cut_pwd_dir(char *pwd);
int		display_prompt(e_prompt prompt);
int		get_stdin(char **line, e_prompt *prompt, t_history **hist, char **env);
void	myhandler_winsize_change(int signal);
int		get_cursor_position(int mode);
void	delete_from_cursor_to_end(t_editor *ed);
void	paste_clipboard(t_editor *ed);
void	save_ed(t_editor *ed, int mode);
void	term_history(t_editor *ed);
int		term_reinit(struct termios *raw_mode);
int		get_term_raw_mode(int mode);
char	*find_env_var(char **env, char *var, int mode);
void	ft_print_git_branch(void);
void	ft_putfreshstr(char *str);

/*
 ** ****************************************************************************
 **                              prompt_git.c                         		   *
 ** ****************************************************************************
 */

void	ft_miniconcat(char **s1, char *s2);
t_editor	*line_editor_init(char **line, e_prompt prompt, int prompt_size, t_history **hist);
int		line_editor_delete(t_editor *ed, t_history **hist);
void	init_t_tab(t_editor *ed);
int		term_size(t_editor *ed);
void	window_resize(t_editor *ed, e_prompt *prompt);
int		print_key(t_editor *ed);

void	del_lines(int nb_line);
void	insert_lines(int nb_line);
t_history	*hist_add(t_history *hist);

/*
 *******************************************************************************
 **                                     TABULATOR                              *
 *******************************************************************************
 */

int		term_tabulator(t_editor *ed, char **env, e_prompt *prompt);
int		tabulator_read(t_tab *tabu, t_editor *ed, int mode);
void	check_data_with_space_after(char **new_cmd, char *d_name);
void	check_data_with_space_before(char **path);
void	tabulator_one_row(t_tab_elem *elem, int max_len);
int		tabulator_multi_row(t_tab *tabu, t_editor *ed);
void	tabulator_recup_data(t_editor *ed, t_tab *tabu);
void	tabulator_recup_folder_files(t_tab *tabu, char *bin);
int		tabulator_get_path(t_editor *ed, t_tab *tabu);
int		nb_line(char *line, int first_char, int ws_col);
int		check_if_name_with_new_line(char *name);
void	tabulator_put_new_cmd(t_tab *tabu, t_editor *ed);

t_history	*init_hist(void);
void		fill_hist_file(t_history *hist);
void	print_line(char *line, int str_pos, int cur_pos, int ws_col);
int		last_char_pos(t_editor *ed);
int		check_if_new_line_in_line(t_editor *ed);
int		get_read_key(char **key);
int		enough_space_on_screen(t_editor *ed);
void	calculate_first_and_last_row(t_editor *ed);

#endif
