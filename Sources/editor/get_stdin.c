/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_stdin.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/12 00:01:33 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 17:15:50 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static void		get_stdin_next(char **line, t_editor **ed, e_prompt *prompt, t_history **hist)
{
	go_to_end_of_line(*ed);
	ft_putchar('\n');
	if (*prompt != PROMPT)
	{
		if ((*ed)->hist->cmd)
			ft_strjoin_free(line, (*ed)->hist->cmd);
	}
	else
		*line = ft_strdup((*ed)->hist->cmd);
	while ((*hist)->next)
		*hist = (*hist)->next;
}

static int		get_keyboard_key_ctrl(t_editor **ed, e_prompt *p)
{
	if (CTRL_D && !(*ed)->hist->cmd)
		(*ed)->ret = -2;
	else if (CTRL_C)
	{
		go_to_end_of_line(*ed);
		(*ed)->ret = -3;
	}
	else if (CTRL_L)
		clear_window(ed, *p);
	else if (CTRL_K && (ft_strlen((*ed)->hist->cmd) + (*ed)->cursor_str_pos))
		delete_from_cursor_to_end(*ed);
	else if (CTRL_P)
		paste_clipboard(*ed);
	return (EXIT_SUCCESS);
}

int				enough_space_on_screen(t_editor *ed)
{
//	if (ed->first_row == )
	if (((ft_strlen(ed->hist->cmd) + ed->prompt_size))
	>= (ed->ws_col * ed->ws_row))
		return (0);
	return (1);
}

static int		get_keyboard_key(t_editor **ed, e_prompt *prompt, char **env)
{
	if (CTRL_D || CTRL_C || CTRL_L || CTRL_K || CTRL_P)
		get_keyboard_key_ctrl(ed, prompt);
	else if (LEFT_KEY || RIGHT_KEY)
		LEFT_KEY ? move_cursor_left(*ed) : move_cursor_right(*ed);
	else if (SHIFT_UP || SHIFT_DOWN)
		SHIFT_UP ? move_cursor_up(*ed) : move_cursor_down(*ed);
	else if ((SHIFT_LEFT || SHIFT_RIGHT) && (*ed)->hist->cmd)
		SHIFT_LEFT ? move_word_left(*ed) : move_word_right(*ed);
	else if (HOME_KEY || CTRL_A || END_KEY || CTRL_E)
		HOME_KEY || CTRL_A ? go_to_begin_of_line(*ed) : go_to_end_of_line(*ed);
	else if (ft_isprint((*ed)->key[0]) && enough_space_on_screen(*ed))
		return (print_key(ed));
	else if (BACKSPACE && (*ed)->hist->cmd && (*ed)->cursor_str_pos)
		backspace(*ed);
	else if ((UP_KEY || DOWN_KEY))
		term_history(ed);
	else if (TAB_KEY)
		(*ed)->ret = term_tabulator(ed, env, prompt);
//	dprintf(2, "get_cur_col: %zu\n", get_cursor_position(0));
//	dprintf(2, "cur_col: %zu\n", (*ed)->cur_col);
//	dprintf(2, "get_cur_row: %zu\n", get_cursor_position(1));
//	dprintf(2, "cur_row: %zu\n", (*ed)->cur_row);
	return (EXIT_SUCCESS);
}

int				get_stdin(char **line, e_prompt *prompt, t_history **hist, char **env)
{
	t_editor	*ed;

	ed = NULL;
	get_term_raw_mode(1);
	if (!(ed = line_editor_init(line, *prompt, display_prompt(*prompt), hist)))
		return (-2);
	term_size(ed);
	while (ed->ret != -1)
	{
		ft_bzero(ed->key, BUFF_READ);
		ed->ret = read(STDIN_FILENO, ed->key, BUFF_READ - 1);
		tputs(tgetstr("vi", NULL), 1, ft_putchar);
		if (term_size(ed) == EXIT_SUCCESS)
			window_resize(&ed, prompt);
		if (ed->ret && get_keyboard_key(&ed, prompt, env))
			ft_strjoin_free(&ed->hist->cmd, ed->key);
			tputs(tgetstr("ve", NULL), 1, ft_putchar);
		if ((ed->key[0] && ((ft_strchr(ed->key, '\n') ||
		(ed->ret == -2 && !ed->hist->cmd)))) || ed->ret == -3)
			break ;
	}
	get_stdin_next(line, &ed, prompt, hist);
	get_term_raw_mode(0);
	return (line_editor_delete(&ed, hist));
}
