/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   escape_sequence.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/17 17:32:51 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/17 17:33:06 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

char			*cursor_position_escape_sequence(int row, int col, t_editor *ed)
{
	char *h_pos;
	char *v_pos;
	char *cursor_pos;

	v_pos = ft_itoa(ed->cur_row - row);
	h_pos = ft_itoa(ed->cur_col - col);
	cursor_pos = ft_strnew(6 + ft_strlen(v_pos) + ft_strlen(h_pos));
	ft_strcpy(cursor_pos, "\E[");
	ft_strcat(cursor_pos, v_pos);
	ft_strcat(cursor_pos, ";");
	ft_strcat(cursor_pos, h_pos);
	ft_strcat(cursor_pos, "H");
	ft_strdel(&h_pos);
	ft_strdel(&v_pos);
	return (cursor_pos);
}

void			reset_cursor_position_escape_sequence(char **cursor_positon)
{
	if (cursor_positon)
	{
		ft_putstr(*cursor_positon);
		ft_strdel(cursor_positon);
	}
}
