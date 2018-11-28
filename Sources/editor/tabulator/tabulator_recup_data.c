/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_recup_data.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/28 12:00:36 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/28 12:15:03 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	tabulator_sort_list(t_tab_elem **list, int nb_elem)
{
	t_dirent	*tmp;
	t_dirent	**e;
	int			i;
	int			j;

	if (!(e = ft_memalloc(sizeof(t_dirent*) * nb_elem)))
		return ;
	i = -1;
	while (*list && ++i < nb_elem && (e[i] = (*list)->dir))
		*list = (i == nb_elem - 1) ? *list : (*list)->next;
	i = -1;
	while (++i < nb_elem && (j = -1))
		while (++j < nb_elem)
			if (ft_strcmp(e[i]->d_name, e[j]->d_name) < 0)
			{
				tmp = e[i];
				e[i] = e[j];
				e[j] = tmp;
			}
	i = nb_elem;
	while (--i >= 0 && (((*list)->dir = e[i])))
		(*list) = (*list)->prev ? (*list)->prev : *list;
	free(e);
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

void	tabulator_get_binairies(char *b_path, t_tab **tabu)
{
	int		i;
	char	**bin;

	i = -1;
	bin = ft_strsplit(b_path, ':');
	while (bin[++i])
		if (((*tabu)->dir = opendir(bin[i])))
		{
			tabulator_recup_folder_files(tabu, bin[i]);
			ft_strdel(&(bin[i]));
		}
	free(bin);
}

void	tabulator_recup_data(t_editor *ed, t_tab **tabu, char *b_path)
{
	if (!((*tabu)->mode = tabulator_get_path(ed, tabu)))
		tabulator_get_binairies(b_path, tabu);
	else if ((*tabu)->mode == 1 || (*tabu)->mode == 2)
	{
		if ((*tabu)->path)
			check_data_with_space_before(&(*tabu)->path);
		if ((*tabu)->path && !(tabulator_open_dir(tabu)) && (*tabu)->mode == 1)
			tabulator_get_binairies(b_path, tabu);
		if ((*tabu)->dir || ((*tabu)->dir = opendir(".")))
			tabulator_recup_folder_files(tabu, NULL);
	}
	(*tabu)->nb_col = (ed->ws_col) / ((*tabu)->max_len + 2);
	(*tabu)->nb_row = (*tabu)->nb_node / (*tabu)->nb_col;
	tabulator_sort_list(&(*tabu)->elem, (*tabu)->nb_node);
	while ((*tabu)->nb_row &&
	((*tabu)->nb_row * (*tabu)->nb_col) < (*tabu)->nb_node)
		(*tabu)->nb_row++;
}
