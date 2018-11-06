/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   go_to_of_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/15 20:32:37 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/06 15:07:05 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	go_to_end_of_line(t_editor *ed)
{
	if (ed->cursor_str_pos < ft_strlen(ed->hist->cmd))
	{
		tputs(tgoto(tgetstr("cv", NULL), 0, ed->last_row - 1), 1, ft_putchar);
		tputs(tgoto(tgetstr("ch", NULL), 0, ed->last_char - 1), 1,
		ft_putchar);
		ed->cursor_str_pos = ft_strlen(ed->hist->cmd);
		ed->cur_col = ed->last_char;
		ed->cur_row = ed->last_row;
	}
}

void	go_to_begin_of_line(t_editor *ed)
{
	if (ed->cursor_str_pos)
	{
		tputs(tgoto(tgetstr("cv", NULL), 0, ed->first_row - 1), 1, ft_putchar);
		tputs(tgoto(tgetstr("ch", NULL), 0,
		ed->first_char - 1), 1, ft_putchar);
		ed->cursor_str_pos = 0;
		ed->cur_col = ed->first_char;
		ed->cur_row = ed->first_row;
	}
}
