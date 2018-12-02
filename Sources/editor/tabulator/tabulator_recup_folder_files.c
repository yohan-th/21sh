/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_recup_folder_files.c                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/28 12:09:31 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/02 17:27:17 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		tabulator_check_executable(t_tab *tabu, t_dirent **dirent, char *bin)
{
	t_stat	buf;
	char	*path;
	DIR		*dir;

	path = ft_strdup(bin ? bin : tabu->path);
	ft_strjoin_free(&path, (*dirent)->d_name);
	lstat(path, &buf);
	if ((dir = opendir(path)) &&
	((*dirent)->d_type = 4))
		closedir(dir);
	ft_strdel(&path);
	if (!tabu->mode || bin || (!bin && (*dirent)->d_type == 4))
		return (1);
	else if (!bin || tabu->mode == 2)
	{
		if (tabu->path && (*dirent)->d_type == 8 && (buf.st_mode & S_IXUSR))
		{
			(*dirent)->d_type = 16;
			return (1);
		}
		if ((buf.st_mode & S_IFMT) == S_IFDIR || tabu->mode == 2)
			return (1);
	}
	return (0);
}

void	tabulator_autocomplete(char **comp, char *d_name)
{
	int i;

	i = 0;
	if (!(*comp))
	{
		*comp = ft_strdup(d_name);
		return ;
	}
	while ((*comp)[i] && d_name[i] && (*comp)[i] == d_name[i])
		i++;
	ft_strdel(comp);
	*comp = ft_strsub(d_name, 0, i);
}

void	tabulator_recup_folder_files(t_tab **tabu, char *bin)
{
	struct dirent		*dir;
	t_tab_elem		*list;
	t_tab_elem		*new;

	list = (*tabu)->last_elem;
	new = NULL;
	while ((dir = readdir((*tabu)->dir)))
	{
		if ((((!(*tabu)->data && ft_strcmp(".", dir->d_name) && ft_strcmp("..", dir->d_name)))
		|| (((bin && ft_strcmp(".", dir->d_name) && ft_strcmp("..", dir->d_name)) || !bin)
		&& (*tabu)->data && !ft_strncmp(dir->d_name, (*tabu)->data, ft_strlen((*tabu)->data))))
		&& tabulator_check_executable(*tabu, &dir, bin))
		{
			if (!(*tabu)->comp || ft_strlen((*tabu)->comp))
			tabulator_autocomplete(&(*tabu)->comp, dir->d_name);
			(*tabu)->max_len = dir->d_namlen > (*tabu)->max_len ? dir->d_namlen : (*tabu)->max_len;
			if (!(new = malloc(sizeof(t_tab_elem))))
				return ;
			new->d_name = ft_strdup(dir->d_name);
			new->d_namlen = dir->d_namlen;
			new->d_type = dir->d_type;
			new->next = NULL;
			new->prev = NULL;
			if (list)
			{
				list->next = new;
				new->prev = list;
			}
			list = new;
			(*tabu)->elem = !(*tabu)->elem ? new : (*tabu)->elem;
			(*tabu)->nb_node++;
		}
	}
	(*tabu)->last_elem = list;
	closedir(((*tabu)->dir));
	(*tabu)->dir = NULL;
}
