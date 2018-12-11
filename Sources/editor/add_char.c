/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   add_char.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/19 10:42:22 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 20:43:04 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static void		add_char_into_line_2(char *key, t_editor *ed)
{
	char *tmp;

	tputs(tgetstr("ei", NULL), 1, ft_putchar);
	tmp = ft_strsub(ed->hist->cmd, 0, ed->cursor_str_pos);
	ft_strjoin_free(&tmp, key);
	ft_strjoin_free(&tmp, ed->hist->cmd + ed->cursor_str_pos);
	ft_strdel(&(ed->hist->cmd));
	ed->hist->cmd = tmp;
	tputs(tgetstr("sc", NULL), 1, ft_putchar);
	ed->cursor_str_pos++;
	write(1, ed->hist->cmd + ed->cursor_str_pos,
	ft_strlen(ed->hist->cmd + ed->cursor_str_pos));
	tputs(tgetstr("rc", NULL), 1, ft_putchar);
	ed->cur_col++;
	ed->last_char++;
}

static void		add_char_into_line_1(char *key, t_editor *ed)
{
	if (ed->last_char == ed->ws_col)
	{
		ed->last_char = 0;
		ft_putchar(key[0]);
		if (ed->cur_col == ed->ws_col)
		{
			ed->cur_col = 0;
			tputs(tgetstr("do", NULL), 1, ft_putchar);
			ed->cur_row = ed->cur_row == ed->ws_row ?
			ed->cur_row : ed->cur_row + 1;
		}
		ed->last_row = ed->last_row == ed->ws_row ?
		ed->last_row : ed->last_row + 1;
	}
	else if (ed->cur_col == ed->ws_col)
	{
		ed->cur_col = 0;
		ft_putchar(key[0]);
		tputs(tgetstr("do", NULL), 1, ft_putchar);
		ed->cur_row = ed->cur_row == ed->ws_row ? ed->cur_row : ed->cur_row + 1;
	}
	else
		ft_putchar(key[0]);
}

void			add_char_into_line(char *key, t_editor *ed)
{
	char *cursor_reset;

	tputs(tgetstr("im", NULL), 1, ft_putchar);
	if (ed->hist->cmd && ed->last_char == ed->ws_col && ed->last_row == ed->ws_row)
	{
		if (ed->cur_col == ed->ws_col)
		{
			ed->cur_col = 0;
			tputs(tgetstr("do", NULL), 1, ft_putchar);
			ed->cur_row = ed->cur_row == ed->ws_row ?
			ed->cur_row : ed->cur_row + 1;
		}
		ft_putchar(key[0]);
		cursor_reset = cursor_position_escape_sequence(1, -1, ed);
		tputs(tgoto(tgetstr("DO", NULL), 0,
		(ed->last_row - ed->cur_row)), 1, ft_putchar);
		tputs(tgetstr("sf", NULL), 1, ft_putchar);
		reset_cursor_position_escape_sequence(&cursor_reset);
		ed->first_row--;
		ed->last_char = 0;
		ed->cur_row--;
	}
	else
		add_char_into_line_1(key, ed);
	add_char_into_line_2(key, ed);
}

void			add_char_to_line(char key, t_editor *ed)
{
	ed->cursor_str_pos++;
	ft_putchar(key);
	if (ed->cur_col == ed->ws_col && ed->cur_row != ed->ws_row)
	{
		ed->cur_col = 0;
		ed->last_char = 0;
		tputs(tgetstr("do", NULL), 1, ft_putchar);
		ed->cur_row++;
		ed->last_row++;
	}
	else if (ed->cur_col == ed->ws_col && ed->cur_row == ed->ws_row)
	{
		ed->cur_col = 0;
		ed->last_char = 0;
		tputs(tgetstr("sf", NULL), 1, ft_putchar);
		ed->first_row--;
	}
	ed->cur_col++;
	ed->last_char++;
}

int				check_if_new_line_in_line(t_editor *ed)
{
	int i;
	int pos;

	i = ed->cursor_str_pos;
	pos = ed->cur_col;
	while (ed->hist->cmd[i])
	{
		if (ed->hist->cmd[i] == '\n')
			return (pos);
		if (pos == (int)ed->ws_col)
			pos = 0;
		i++;
		pos++;
	}
	return (-1);
}

void			print_line(t_editor *ed)
{
	int i;
	int cur_pos;

	i = 0;
	cur_pos = ed->first_char - 1;
	while (ed->hist->cmd && ed->hist->cmd[i])
	{
		if (ed->hist->cmd[i] != '\n')
			ft_putchar(ed->hist->cmd[i]);
		if (ed->hist->cmd[i] == '\n' || (cur_pos + 1) == (int)ed->ws_col)
		{
			tputs(tgetstr("sf", NULL), 1, ft_putchar);
			tputs(tgetstr("cr", NULL), 1, ft_putchar);
			cur_pos = 0;
		}
		else
			cur_pos++;
		i++;
	}
}

void			add_char_into_line_with_new_line(t_editor **ed, int pos)
{
	char *tmp;
	int tmp_cur_col;
	int tmp_cur_row;
	int tmp_cursor_pos;

	tmp_cursor_pos = (*ed)->cursor_str_pos;
	tmp_cur_col = (*ed)->cur_col;
	tmp_cur_row = (*ed)->cur_row;
	if ((*ed)->cur_col == (*ed)->ws_col)
	{
		tmp_cur_col = 0;
		tmp_cur_row++;
	}
	if (pos == (int)(*ed)->ws_col)
	{
		if ((*ed)->last_row == (*ed)->ws_row)
		{
			go_to_end_of_line(*ed);
			tmp_cur_row--;
			(*ed)->first_row--;
			tputs(tgetstr("sf", NULL), 1, ft_putchar);
		}
		else
			(*ed)->last_row++;
	}
	tmp = ft_strsub((*ed)->hist->cmd, 0, tmp_cursor_pos);
	ft_strjoin_free(&tmp, (*ed)->key);
	ft_strjoin_free(&tmp, (*ed)->hist->cmd + tmp_cursor_pos);
	tmp_cursor_pos++;
	ft_strdel(&(*ed)->hist->cmd);
	(*ed)->hist->cmd = tmp;
	go_to_begin_of_line(*ed);
	ft_putstr("\E[J");
	print_line(*ed);
	(*ed)->cur_col = tmp_cur_col;
	(*ed)->cur_row = tmp_cur_row;
	(*ed)->cursor_str_pos = tmp_cursor_pos;
	tputs(tgoto(tgetstr("cm", NULL), (*ed)->cur_col, (*ed)->cur_row - 1), 1, ft_putchar);
	(*ed)->cur_col++;
}

int				print_key(t_editor **ed)
{
	int pos;

	if (ft_strlen((*ed)->key) == 1)
	{
		if ((int)(*ed)->cursor_str_pos == ft_strlen((*ed)->hist->cmd))
		{
			add_char_to_line((*ed)->key[0], *ed);
			return (1);
		}
		else
		{
			if ((pos = check_if_new_line_in_line(*ed)) != -1)
				add_char_into_line_with_new_line(ed, pos);
			else
				add_char_into_line((*ed)->key, *ed);
		}
	}
	else
		add_paste_into_line(ed);
	return (EXIT_SUCCESS);
}
