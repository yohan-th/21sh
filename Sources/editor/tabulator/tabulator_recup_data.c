/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_recup_data.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/28 12:00:36 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/30 15:32:58 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	tabulator_sort_list(t_tab_elem **list)
{
	t_tab_elem *tmp;

	while ((*list) && (*list)->next)
	{
		if (ft_strcmp((*list)->d_name, (*list)->next->d_name) > 0)
		{
			tmp = *list;
			*list = (*list)->next;
			tmp->next = (*list)->next;
			if (tmp->next)
				tmp->next->prev = tmp;
			(*list)->prev = tmp->prev;
			tmp->prev = *list;
			(*list)->next = tmp;
			if ((*list)->prev)
				(*list)->prev->next = *list;
			if ((*list)->prev)
				*list = (*list)->prev;
		}
		else
			*list = (*list)->next;
	}
	while ((*list) && (*list)->prev)
		*list = (*list)->prev;
}

int		tabulator_open_dir(t_tab **tabu)
{
	int		len;
	char	tmp[ft_strlen((*tabu)->path) + 1];

	len = ft_strlen((*tabu)->path);
	while (len)
	{
		ft_bzero(tmp, len + 1);
		ft_strncpy(tmp, (*tabu)->path, len);
		if (tmp[len - 1] == '/' && ((*tabu)->dir = opendir(tmp)))
		{
			(*tabu)->data = ft_strlen((*tabu)->path + len) ? ft_strdup((*tabu)->
			path + len) : (*tabu)->data;
			ft_strdel(&(*tabu)->path);
			(*tabu)->path = ft_strdup(tmp);
			return (1);
		}
		len--;
	}
	(*tabu)->data = ft_strdup((*tabu)->path);
	ft_strdel(&(*tabu)->path);
	return (0);
}

void	tabulator_get_binairies(t_tab **tabu)
{
	int		i;
	char	*b_path;
	char	**bin;

	i = -1;
	if (!(b_path = get_envp((*tabu)->env, "PATH")))
		return ;
	bin = ft_strsplit(b_path, ':');
	ft_strdel(&b_path);
	while (bin[++i])
	{
		if (((*tabu)->dir = opendir(bin[i])))
			tabulator_recup_folder_files(tabu, bin[i]);
		ft_strdel(&(bin[i]));
	}
	ft_strdel(&(bin[i]));
	free(bin);
}

void	tabulator_recup_data(t_editor *ed, t_tab **tabu)
{
	if (!((*tabu)->mode = tabulator_get_path(ed, tabu)))
		tabulator_get_binairies(tabu);
	else if ((*tabu)->mode == 1 || (*tabu)->mode == 2)
	{
		if ((*tabu)->path)
			check_data_with_space_before(&(*tabu)->path);
		if ((*tabu)->path && !(tabulator_open_dir(tabu)) && (*tabu)->mode == 1)
			tabulator_get_binairies(tabu);
		if ((*tabu)->dir || ((*tabu)->dir = opendir(".")))
			tabulator_recup_folder_files(tabu, NULL);
	}
	(*tabu)->nb_col = (ed->ws_col) / ((*tabu)->max_len + 2);
	(*tabu)->nb_row = (*tabu)->nb_node / (*tabu)->nb_col;
	if ((*tabu)->elem)
		tabulator_sort_list(&(*tabu)->elem);
	while ((*tabu)->nb_row &&
	((*tabu)->nb_row * (*tabu)->nb_col) < (*tabu)->nb_node)
		(*tabu)->nb_row++;
	if ((*tabu)->comp && ((*tabu)->nb_node == 1 ||
	ft_strlen((*tabu)->comp) == ft_strlen((*tabu)->data)))
		ft_strdel(&(*tabu)->comp);
}
