/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   term_tabulator.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/07 16:25:14 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/03 14:58:44 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

t_tab	*tabulator_init(int cursor_pos, char **env)
{
	t_tab *tabu;

	if (!(tabu = (t_tab*)malloc(sizeof(t_tab))))
		return (NULL);
	tabu->path = NULL;
	tabu->data = NULL;
	tabu->home = NULL;
	tabu->comp = NULL;
	tabu->dir = NULL;
	tabu->elem = NULL;
	tabu->last_elem = NULL;
	tabu->env = env;
	tabu->nb_col = 0;
	tabu->nb_row = 0;
	tabu->max_len = 0;
	tabu->nb_node = 0;
	tabu->mode = 0;
	tabu->start = cursor_pos;
	tabu->end = cursor_pos;
	return (tabu);
}

void	tabulator_put_new_cmd(t_tab **tabu, t_editor **ed)
{
	int tmp_pos;
	char *new;

	new = NULL;
	new = ft_strsub((*ed)->hist->cmd, 0, (*tabu)->start);
	ft_strjoin_free(&(*tabu)->data, ((*tabu)->comp ?
	(*tabu)->comp : (*tabu)->elem->d_name) + ft_strlen((*tabu)->data));
	ft_strjoin_free(&(*tabu)->path, (*tabu)->data);
	check_data_with_space_after(&new, (*tabu)->path);
	if ((*tabu)->elem->d_type == 4 && (*tabu)->nb_node == 1)
		ft_strjoin_free(&new, "/");
	else if (!ft_strlen((*ed)->hist->cmd + (*ed)->cursor_str_pos) &&
	(*tabu)->nb_node == 1)
		ft_strjoin_free(&new, " ");
	tmp_pos = ft_strlen(new);
	ft_strjoin_free(&new, (*ed)->hist->cmd + (*ed)->cursor_str_pos);
	go_to_begin_of_line(*ed);
	ft_putstr("\E[J");
	if ((*ed)->hist->cmd)
		ft_strdel(&(*ed)->hist->cmd);
	(*ed)->hist->cmd = new;
	ft_putstr(new);
	(*ed)->cursor_str_pos = ft_strlen(new);
	(*ed)->cur_col = get_cursor_position(0);
	(*ed)->last_char = (*ed)->cur_col;
	while ((*ed)->cursor_str_pos > tmp_pos)
		move_cursor_left(*ed);
}

int		tabulator_read(t_tab *tabu, t_editor **ed, int mode)
{
	int ret;

	!mode ? ft_printf("Display all %d possibilities? (y or n)", tabu->nb_node)
	: ft_putstr("--More--");
	tputs(tgetstr("ve", NULL), 1, ft_putchar);
	while ((ret = read(0, (*ed)->key, 1)) > -1)
		if (!mode && ret == 1 && (*ed)->key[0] == 'y' && write(1, "\n", 1))
			return (1);
		else if (!mode && ret == 1 && (*ed)->key[0] == 'n' && write(1, "\n", 1))
			return (0);
		else if (mode && ret == 1 && (*ed)->key[0] == '\n')
			break ;
		else if (term_size(*ed) == EXIT_SUCCESS)
			continue ;
		else if (CTRL_C)
			return (-3);
	tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_putchar);
	ft_putstr("\E[J");
	tabu->nb_node++;
	return (ret);
}

int		nb_line(t_editor *ed)
{
	int i;
	int pos;
	int row;

	i = 0;
	pos = get_cursor_position(0);
	row = 0;
	while (ed->hist->cmd && ed->hist->cmd[i])
	{
		if (pos == (int)ed->ws_col || ed->hist->cmd[i] == '\n')
		{
			pos = 0;
			row++;
		}
		i++;
		pos++;
	}
	return (row);
}

int		tabulator_put_row(t_editor **ed, t_tab *tabu, e_prompt *prompt)
{
	int		nb_l;
	int		ret;

	ret = EXIT_SUCCESS;
	go_to_end_of_line(*ed);
	write(1, "\n", 1);
	if (!tabu->nb_row && tabu->nb_node > 1)
		tabulator_one_row(tabu);
	else if (((ret = tabulator_multi_row(tabu, ed)) == -1 || ret == -3))
		return (ret);
	tputs(tgetstr("vi", NULL), 1, ft_putchar);
	(*ed)->prompt_size = display_prompt(*prompt);
	nb_l = nb_line(*ed);
	ft_putstr((*ed)->hist->cmd);
	(*ed)->last_row = get_cursor_position(1);
	(*ed)->first_row = (*ed)->first_row - nb_l;
	(*ed)->cur_row = (*ed)->last_row;
	(*ed)->cursor_str_pos = ft_strlen((*ed)->hist->cmd);
	if (get_cursor_position(0) == (*ed)->ws_col
	&& (*ed)->cur_col < (*ed)->ws_col)
		tputs(tgetstr("do", NULL), 1, ft_putchar);
	(*ed)->cur_col = (*ed)->last_char;
	while ((*ed)->cursor_str_pos > tabu->save_pos)
		move_cursor_left(*ed);
	return (ret);
}

void	free_tab(t_tab **tabu)
{
	t_tab_elem *tmp;

	ft_strdel(&(*tabu)->path);
	ft_strdel(&(*tabu)->data);
	ft_strdel(&(*tabu)->comp);
	ft_strdel(&(*tabu)->home);
	if ((*tabu)->dir)
		closedir((*tabu)->dir);
	while ((*tabu)->elem)
	{
		tmp = (*tabu)->elem->next;
		if ((*tabu)->elem->d_name)
			ft_strdel(&(*tabu)->elem->d_name);
		free((*tabu)->elem);
		(*tabu)->elem = tmp;
	}
	free(*tabu);
}

int		term_tabulator(t_editor **ed, char **env, e_prompt *prompt)
{
	t_tab *tabu;
	int ret;

	ret = EXIT_SUCCESS;
	tabu = tabulator_init((*ed)->cursor_str_pos, env);
	tabulator_recup_data(*ed, &tabu);
	if (tabu->nb_node)
	{
		if (tabu->nb_node == 1 || tabu->comp)
			tabulator_put_new_cmd(&tabu, ed);
		else
			ret = tabulator_put_row(ed, tabu, prompt);
	}
	free_tab(&tabu);
	return (ret);
}
