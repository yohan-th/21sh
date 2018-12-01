/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   window_resize.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/06 21:40:31 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/01 09:58:39 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	window_resize(t_editor **ed, e_prompt *prompt)
{
	int	cursor_str_pos_tmp;

	cursor_str_pos_tmp = 0;
	cursor_str_pos_tmp = (*ed)->cursor_str_pos;
	tputs(tgetstr("cl", NULL), 1, ft_putchar);
	display_prompt(*prompt);
	(*ed)->first_char = get_cursor_position(0);
	(*ed)->first_row = get_cursor_position(1);
	ft_putstr((*ed)->hist->cmd);
	(*ed)->last_row = get_cursor_position(1);
	(*ed)->last_char = get_cursor_position(0);
	(*ed)->cursor_str_pos = ft_strlen((*ed)->hist->cmd);
	while ((*ed)->cursor_str_pos != cursor_str_pos_tmp)
		move_cursor_left(*ed);
	(*ed)->cur_row = get_cursor_position(1);
	(*ed)->cur_col = get_cursor_position(0);
}

int		term_size(t_editor *ed)
{
	t_sz	sz;

	ft_memset(&sz, 0, sizeof(sz));
	if (ioctl(0, TIOCGWINSZ, &sz) == -1)
		return (EXIT_FAILURE);
	if (ed->ws_col == sz.ws_col && ed->ws_row == sz.ws_row)
		return (EXIT_FAILURE);
	ed->ws_col = sz.ws_col;
	ed->ws_row = sz.ws_row;
	return (EXIT_SUCCESS);
}
