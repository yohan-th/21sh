/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   move_cursor.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/19 04:48:57 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 23:09:55 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		check_next_pos(t_editor *ed)
{
	int row_nb;
	int cur_col;
	int pos;

	row_nb = 0;
	cur_col = ed->cur_col;
	pos = ed->cursor_str_pos;
	while (ed->hist->cmd[pos])
	{
		if (ed->hist->cmd[pos] == '\n' || cur_col == ed->ws_col)
		{
			if (row_nb && ed->hist->cmd[pos] == '\n')
				break ;
			row_nb++;
			cur_col = 0;
		}
		cur_col++;
		pos++;
		if (cur_col == ed->cur_col)
			break ;
	}
	return (pos);
}

int		check_prev_pos(t_editor *ed)
{
	int pos;
	int cur_row;
	int cur_col;

	pos = 0;
	cur_row = ed->first_row;
	cur_col = ed->first_char;
	while (ed->hist->cmd[pos])
	{
		if (cur_col == ed->ws_col || ed->hist->cmd[pos] == '\n')
		{
			cur_col = 0;
			cur_row++;
		}
		cur_col++;
		pos++;
		if (cur_row == ed->cur_row - 1 && ((ed->hist->cmd[pos] == '\n' &&
		cur_col <= ed->cur_col) || cur_col == ed->cur_col))
			break ;
	}
	return (pos);
}

void	move_cursor_up(t_editor *ed)
{
	int pos;

	if (ed->cur_row > ed->first_row)
	{
		if (ed->cur_row - 1 == ed->first_row && ed->first_char >= ed->cur_col)
			pos = 0;
		else
			pos = check_prev_pos(ed);
		while (pos < ed->cursor_str_pos)
			move_cursor_left(ed);
	}
}

void	move_cursor_down(t_editor *ed)
{
	int pos;

	if (ed->cur_row < ed->last_row)
	{
		pos = check_next_pos(ed);
		while (pos > ed->cursor_str_pos)
			move_cursor_right(ed);
	}
}

void	move_cursor_right(t_editor *ed)
{
	if ((int)ed->cursor_str_pos < ft_strlen(ed->hist->cmd))
	{
		if (ed->hist->cmd[ed->cursor_str_pos] == '\n' ||
		ed->cur_col == ed->ws_col)
		{
			ed->cur_col = 0;
			ed->cur_row++;
			tputs(tgetstr("do", NULL), 1, ft_putchar);
		}
		else
			tputs(tgetstr("nd", NULL), 1, ft_putchar);
		ed->cur_col++;
		ed->cursor_str_pos++;
	}
}

void	move_cursor_left(t_editor *ed)
{
	int tmp_pos;

	if (ed->cursor_str_pos > 0 && (tmp_pos = ed->cursor_str_pos - 1) >= 0)
	{
		ed->cursor_str_pos--;
		ed->cur_col--;
		if (ed->cur_col == 0)
		{
			if (ed->hist->cmd[ed->cursor_str_pos] == '\n')
			{
				go_to_begin_of_line(ed);
				while (ed->cursor_str_pos < tmp_pos)
					move_cursor_right(ed);
			}
			else
			{
				ed->cur_col = ed->ws_col;
				ed->cur_row--;
				tputs(tgoto(tgetstr("cm", NULL), ed->ws_col - 1,
				ed->cur_row - 1), 1, ft_putchar);
			}
		}
		else
			tputs(tgetstr("le", NULL), 1, ft_putchar);
	}
}
