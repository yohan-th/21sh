/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   term_history_incremental_search.c                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/03/02 16:23:00 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/02 17:59:51 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"


int				term_history_incremental_search(t_editor *ed)
{
	int i;
	int len;

	i = 0;
	(void)ed;

	ed->rev_hist = 1;
	go_to_begin_of_line(ed);
	ed->cursor_str_pos = ed->prompt_size;
	while (ed->cursor_str_pos)
		move_cursor_left(ed);
	dprintf(2, "cur_col: %d\ncur_row: %d\n", get_cursor_position(0), get_cursor_position(1));
	ft_putstr("\E[0J");
	len = write(1, "(reverse-i-search)`': ", ft_strlen("(reverse-i-search)`': "));
	ed->cursor_str_pos = ft_strlen(ed->hist->cmd);
	while (len)
	{
		if (ed->cur_col == ed->ws_col)
			ed->cur_col = 0;
		ed->cur_col++;
		len--;
	}
	dprintf(2, "len: %d\ned->cur_col: %d\ncur_col: %d\n", len, ed->cur_col, get_cursor_position(0));
	return (0);
}
