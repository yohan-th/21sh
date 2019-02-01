/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_tools.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/01 23:43:13 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/01 23:43:13 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	free_tab(t_tab *tabu)
{
	t_tab_elem *tmp;

	ft_strdel(&tabu->path);
	ft_strdel(&tabu->data);
	ft_strdel(&tabu->comp);
	ft_strdel(&tabu->home);
	if (tabu->dir)
		closedir(tabu->dir);
	while (tabu->elem)
	{
		tmp = tabu->elem->next;
		ft_strdel(&tabu->elem->d_name);
		ft_strdel(&tabu->elem->path);
		free(tabu->elem);
		tabu->elem = tmp;
	}
	free(tabu);
}

int		term_tabulator_enough_space(t_editor *ed, t_tab *tabu)
{
	int new_line_pos;

	new_line_pos = check_if_new_line_in_line(ed);
	if ((ft_strlen(ed->hist->cmd) + ed->prompt_size +
	ft_strlen(tabu->elem->d_name)) >= (ed->ws_col * ed->ws_row) ||
	((ed->first_row == 1 && ed->last_row == ed->ws_row) &&
	(new_line_pos > -1 ? new_line_pos : ed->last_char)
	+ (ft_strlen(tabu->elem->d_name) - 1) >= ed->ws_col))
		return (0);
	return (1);
}
