/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   move_word.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/19 04:54:46 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 23:34:38 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int		check_if_previous_word(char *line, size_t cursor_str_pos)
{
	size_t s_pos;

	s_pos = cursor_str_pos;
	while (cursor_str_pos > 0)
	{
		if ((line[cursor_str_pos - 1] == ' ' || line
		[cursor_str_pos - 1] == '\t' || line[cursor_str_pos - 1] == '\n')
		&& line[cursor_str_pos] >= 33
		&& line[cursor_str_pos] <= 126 && cursor_str_pos != s_pos)
			return (cursor_str_pos);
		cursor_str_pos--;
		if (!cursor_str_pos && line[cursor_str_pos] >= 33
		&& line[cursor_str_pos] <= 126)
			return (cursor_str_pos);
	}
	return (-1);
}

static int		check_if_next_word(char *line)
{
	int cursor_str_pos;

	cursor_str_pos = 0;
	while (line[cursor_str_pos])
	{
		if (cursor_str_pos && (line[cursor_str_pos - 1] == ' ' || line
		[cursor_str_pos - 1] == '\t' || line[cursor_str_pos - 1] == '\n')
		&& line[cursor_str_pos] >= 33
		&& line[cursor_str_pos] <= 126)
			return (cursor_str_pos);
		cursor_str_pos++;
	}
	return (0);
}

void			move_word_left(t_editor *ed)
{
	int	previous_word_pos;

	if ((previous_word_pos =
	check_if_previous_word(ed->hist->cmd, ed->cursor_str_pos)) != -1)
		while (ed->cursor_str_pos > previous_word_pos)
			move_cursor_left(ed);
}

void			move_word_right(t_editor *ed)
{
	int next_word_pos;

	next_word_pos = check_if_next_word(ed->hist->cmd + ed->cursor_str_pos);
	if (next_word_pos)
	{
		next_word_pos += ed->cursor_str_pos;
		while (ed->cursor_str_pos != next_word_pos)
			move_cursor_right(ed);
	}
}
