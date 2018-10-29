/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_stdin.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/12 00:01:33 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/29 22:57:28 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static void		get_stdin_next(char **line, t_editor *ed, e_prompt *prompt)
{
	if ((ed->last_row - ed->cur_row) != 0)
		tputs(tgoto(tgetstr("DO", NULL), 0,
		ed->last_row - ed->cur_row), 1, ft_putchar);
	ft_putchar('\n');
	if (*prompt != PROMPT && *prompt != E_PIPE)
	{
		dprintf(2, "YEAH!!!!!!\n");
		*line = ed->line == NULL ? *line : ft_strjoin_free(*line, ed->line);
	}
	else
	{
		dprintf(2, "NOOOOOOOOO!!1!!\n");
		*line = ft_strdup(ed->line);
		dprintf(2, "YESSSSSSSSSSS!!1!!\n");
	}
}

static int		get_keyboard_key_ctrl(t_editor **ed, e_prompt *p)
{
	if (CTRL_D && !(*ed)->line)
		(*ed)->ret = -2;
	else if (CTRL_C)
		end_of_text(*ed, p);
	else if (CTRL_L)
		clear_window(*ed, *p);
	else if (CTRL_K && (ft_strlen((*ed)->line) + (*ed)->cursor_str_pos))
		delete_from_cursor_to_end(*ed);
	else if (CTRL_P)
		paste_clipboard(*ed);
	return (EXIT_SUCCESS);
}

int				enough_space_on_screen(t_editor *ed)
{
	dprintf(2, "max_char: %zu\n", ed->ws_col * ed->ws_row);
	dprintf(2, "line_len: %zu\n", ed->first_char * ed->first_row);
	return (1);
}

static int		get_keyboard_key(t_editor **ed, e_prompt *prompt)
{
	if (CTRL_D || CTRL_C || CTRL_L || CTRL_K || CTRL_P)
		get_keyboard_key_ctrl(ed, prompt);
	else if (LEFT_KEY || RIGHT_KEY)
		LEFT_KEY ? move_cursor_left(*ed) : move_cursor_right(*ed);
	else if (SHIFT_UP || SHIFT_DOWN)
		SHIFT_UP ? move_cursor_up(*ed) : move_cursor_down(*ed);
	else if ((SHIFT_LEFT || SHIFT_RIGHT) && (*ed)->line)
		SHIFT_LEFT ? move_word_left(*ed) : move_word_right(*ed);
	else if (HOME_KEY || CTRL_A || END_KEY || CTRL_E)
		HOME_KEY || CTRL_A ? go_to_begin_of_line(*ed) : go_to_end_of_line(*ed);
	else if (ft_isprint((*ed)->key[0]) /*&& enough_space_on_screen(*ed)*/)
		return (print_key(ed));
	else if (BACKSPACE && (*ed)->line && (*ed)->cursor_str_pos)
		backspace(*ed);
	return (EXIT_SUCCESS);
}

int				get_stdin(char **line, e_prompt *prompt)
{
	t_editor	*ed;

	ed = NULL;
	get_term_raw_mode(1);
	//display_prompt(*prompt);
	if ((ed = line_editor_init(line, *prompt, display_prompt(*prompt))) == NULL)
		return (-2);
	term_size(ed);
	while (ed->ret != -1)
	{
		ft_bzero(ed->key, BUFF_SIZE);
		ed->ret = read(STDIN_FILENO, ed->key, BUFF_SIZE - 1);
		tputs(tgetstr("vi", NULL), 1, ft_putchar);
		if (term_size(ed) == EXIT_SUCCESS)
			window_resize(&ed, prompt);
		if (ed->ret && get_keyboard_key(&ed, prompt))
			ed->line = ft_strjoin_free(ed->line, ed->key);
		tputs(tgetstr("ve", NULL), 1, ft_putchar);
		if (ed->key[0] && ((ft_strchr(ed->key, '\n') ||
			(ed->ret == -2 && !ed->line) || (ed->ret == -3 && *prompt == E_HDOC))))
		{
			dprintf(2, "OKKKKKKKK1111111111\n");
			break ;
		}
	}
	dprintf(2, "OKKKKKKKK2222222222\n");
	get_stdin_next(line, ed, prompt);
	dprintf(2, "OKKKKKKKK33333333333\n");
	get_term_raw_mode(0);
	dprintf(2, "OKKKKKKKK4444444444444\n");
	return (line_editor_delete(&ed));
}
