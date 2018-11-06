/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   delete_from_cursor_to_end.c                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/16 14:58:15 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/06 15:09:59 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void			delete_from_cursor_to_end(t_editor *ed)
{
	char tmp[ed->cursor_str_pos + 1];

	ft_strdel(&ed->clipboard);
	ft_bzero(tmp, sizeof(tmp));
	ft_putstr("\E[0J");
	if (!ed->cursor_str_pos)
	{
		ed->clipboard = ft_strdup(ed->hist->cmd);
		ft_strdel(&(ed->hist->cmd));
	}
	else
	{
		ft_strncpy(tmp, ed->hist->cmd, ed->cursor_str_pos);
		ed->clipboard = ft_strdup(ed->hist->cmd + ed->cursor_str_pos);
		ft_strdel(&(ed->hist->cmd));
		ed->hist->cmd = ft_strdup(tmp);
	}
}
