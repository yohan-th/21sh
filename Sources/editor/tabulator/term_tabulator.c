/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   term_tabulator.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/07 16:25:14 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/30 15:33:04 by dewalter    ###    #+. /#+    ###.fr     */
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
	char *new;

	new = NULL;
	dprintf(2, "comp: %s\n", (*tabu)->comp);
	new = ft_strsub((*ed)->hist->cmd, 0, (*tabu)->start);
	(*tabu)->data = ft_strjoin_free((*tabu)->data, ((*tabu)->comp ?
	(*tabu)->comp : (*tabu)->elem->d_name) + ft_strlen((*tabu)->data));
	(*tabu)->path = ft_strjoin_free((*tabu)->path, (*tabu)->data);
	check_data_with_space_after(&new, (*tabu)->path);
	if ((*tabu)->elem->d_type == 4 && (*tabu)->nb_node == 1)
		new = ft_strjoin_free(new, "/");
	else if (!ft_strlen((*ed)->hist->cmd + (*ed)->cursor_str_pos) &&
	(*tabu)->nb_node == 1)
		new = ft_strjoin_free(new, " ");
	new = ft_strjoin_free(new, (*ed)->hist->cmd + (*ed)->cursor_str_pos);
	go_to_begin_of_line(*ed);
	ft_putstr("\E[J");
	if ((*ed)->hist->cmd)
		ft_strdel(&(*ed)->hist->cmd);
	(*ed)->hist->cmd = new;
	ft_putstr(new);
	(*ed)->cursor_str_pos = ft_strlen(new);
	(*ed)->cur_col = get_cursor_position(0);
	(*ed)->last_char = (*ed)->cur_col;
}

int		tabulator_read(t_tab *tabu, t_editor **ed, e_prompt *prompt, int mode)
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
		else if (CTRL_C)
		{
			end_of_text(ed, prompt);
			return (-1);
		}
	tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_putchar);
	ft_putstr("\E[J");
	tabu->nb_node++;
	return (ret);
}

void	term_tabulator(t_editor **ed, char **env, e_prompt *prompt)
{
	int		nb_line;
	int		cur_row;
	t_tab *tabu;

	tabu = tabulator_init((*ed)->cursor_str_pos, env);
	tabulator_recup_data(*ed, &tabu);
	dprintf(2, "comp: %s\n", tabu->comp);
	if (!tabu->nb_node)
		return ;
	else if (tabu->nb_node == 1 || tabu->comp)
		tabulator_put_new_cmd(&tabu, ed);
	else
	{
		ft_putchar('\n');
		if (!tabu->nb_row && tabu->nb_node > 1)
			tabulator_put_one_row(tabu);
		else if (tabulator_put_multi_row(tabu, ed, prompt) == -1)
			return ;
		nb_line = (*ed)->last_row - (*ed)->first_row;
		cur_row = (*ed)->cur_row - (*ed)->first_row;
		if (!tabu->nb_row)
			tabu->nb_row++;
		if (((*ed)->last_row + tabu->nb_row + nb_line) > (*ed)->ws_row)
		{
			(*ed)->first_row = (*ed)->ws_row - nb_line;
			(*ed)->cur_row = (*ed)->first_row + cur_row;
			(*ed)->last_row = (*ed)->ws_row;
		}
		else
		{
			(*ed)->first_row = (*ed)->last_row + tabu->nb_row + 1;
			(*ed)->cur_row = (*ed)->first_row + cur_row;
			(*ed)->last_row = (*ed)->first_row + nb_line;
		}
		(*ed)->cursor_str_pos = ft_strlen((*ed)->hist->cmd);
		display_prompt(*prompt);
		ft_putstr((*ed)->hist->cmd);
		ft_bzero((*ed)->key, BUFF_READ);
	}
}
