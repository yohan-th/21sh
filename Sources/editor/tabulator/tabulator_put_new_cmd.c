/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_put_new_cmd.c                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/17 22:13:29 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/19 21:34:01 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	add_quote_to_data(t_tab *tabu)
{
	char *tmp;

	tmp = ft_strdup("'");
	ft_strjoin_free(&tmp, tabu->elem->d_name);
	ft_strjoin_free(&tmp, "'");
	ft_strdel(&tabu->elem->d_name);
	tabu->elem->d_name = tmp;
}

void	tabulator_put_new_cmd_next(t_editor *ed, char **new)
{
	int	tmp_pos;

	tmp_pos = ft_strlen(*new);
	ft_strjoin_free(new, ed->hist->cmd + ed->cursor_str_pos);
	go_to_begin_of_line(ed);
	ft_putstr("\E[J");
	ft_strdel(&ed->hist->cmd);
	ed->hist->cmd = *new;
	print_line(ed->hist->cmd, 0, ed->first_char - 1, ed->ws_col);
	ed->cursor_str_pos = ft_strlen(*new);
	ed->cur_col = last_char_pos(ed);
	ed->last_char = ed->cur_col;
	calculate_first_and_last_row(ed);
	ed->cur_row = ed->last_row;
	while (ed->cursor_str_pos > tmp_pos)
		move_cursor_left(ed);
}

void	tabulator_put_new_cmd(t_tab *tabu, t_editor *ed)
{
	DIR		*dir;
	char	*new;

	new = ft_strsub(ed->hist->cmd, 0, tabu->start);
	if (tabu->comp)
		ft_strjoin_free(&tabu->data, tabu->comp + ft_strlen(tabu->data));
	else if (check_if_name_with_new_line(tabu->elem->d_name))
	{
		add_quote_to_data(tabu);
		ft_strdel(&tabu->data);
		tabu->data = ft_strdup(tabu->elem->d_name);
	}
	else
		ft_strjoin_free(&tabu->data,
		tabu->elem->d_name + ft_strlen(tabu->data));
	ft_strjoin_free(&tabu->path, tabu->data);
	check_data_with_space_after(&new, tabu->path);
	if (tabu->nb_node == 1 && (dir = opendir(tabu->path)) && !closedir(dir))
		ft_strjoin_free(&new, "/");
	else if (!ft_strlen(ed->hist->cmd + ed->cursor_str_pos) &&
		tabu->nb_node == 1)
		ft_strjoin_free(&new, " ");
	tabulator_put_new_cmd_next(ed, &new);
}
