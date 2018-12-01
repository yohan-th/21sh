/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   clear_window.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/16 15:01:15 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/01 09:58:58 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int				clear_window(t_editor *ed, e_prompt prompt)
{
	int	cursor_str_pos_tmp;

	cursor_str_pos_tmp = ed->cursor_str_pos;
	tputs(tgetstr("cl", NULL), 1, ft_putchar);
	display_prompt(prompt);
	ed->first_row = get_cursor_position(1);
	ed->first_char = get_cursor_position(0);
	if (ed->hist->cmd)
		ft_putstr(ed->hist->cmd);
	ed->cur_col = get_cursor_position(0);
	ed->cur_row = get_cursor_position(1);
	ed->last_row = get_cursor_position(1);
	ed->last_char = get_cursor_position(0);
	ed->cursor_str_pos = ft_strlen(ed->hist->cmd);
	while (cursor_str_pos_tmp != ed->cursor_str_pos)
		move_cursor_left(ed);
	return (0);
}
