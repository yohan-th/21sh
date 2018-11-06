/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   paste_line.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/15 20:34:38 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/06 15:11:32 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	add_paste_into_line(t_editor *ed)
{
	size_t	tmp_last_pos;
	char	tmp[ft_strlen(ed->hist->cmd) + ft_strlen(ed->key) + 1];

	tmp_last_pos = ed->cursor_str_pos + ft_strlen(ed->key);
	if (ed->hist->cmd)
	{
		ft_bzero(tmp, ft_strlen(ed->hist->cmd) + ft_strlen(ed->key) + 1);
		ft_strncpy(tmp, ed->hist->cmd, ed->cursor_str_pos);
		ft_strcat(tmp, ed->key);
		ft_strcat(tmp, ed->hist->cmd + ed->cursor_str_pos);
		ft_strdel(&ed->hist->cmd);
		ed->hist->cmd = ft_strdup(tmp);
	}
	else
	{
		ft_strdel(&ed->hist->cmd);
		ed->hist->cmd = ft_strdup(ed->key);
	}
	ft_putstr(ed->hist->cmd + ed->cursor_str_pos);
	ed->last_char = get_cursor_position(0);
	ed->last_row = get_cursor_position(1);
	ed->cursor_str_pos = ft_strlen(ed->hist->cmd);
	while (ed->cursor_str_pos != tmp_last_pos)
		move_cursor_left(ed);
	if (ed->cur_col == ed->ws_col && ed->cur_row == ed->ws_row)
		tputs(tgetstr("sf", NULL), 1, ft_putchar);
}
