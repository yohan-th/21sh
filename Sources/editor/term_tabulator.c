/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   term_tabulator.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/07 16:25:14 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/21 18:08:52 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	tabulator_recup_directory_files(t_tab **tabu)
{
	t_dirent		*dirent;
	t_tab_elem		*list;
	t_tab_elem		*new;

	list = (*tabu)->last_elem;
	new = NULL;
	while ((dirent = readdir((*tabu)->dir)))
	{
		if (((!(*tabu)->mode || !(*tabu)->data) && (ft_strcmp(".", dirent->d_name) && ft_strcmp("..",
		dirent->d_name))) || ((*tabu)->data && !ft_strncmp(dirent->d_name, (*tabu)->data, ft_strlen((*tabu)->data))))
		{
			(*tabu)->max_len = dirent->d_namlen > (*tabu)->max_len ? dirent->d_namlen : (*tabu)->max_len;
			if (!(new = malloc(sizeof(t_tab_elem))))
				return ;
			new->dir = malloc(sizeof(t_dirent));
			*(new->dir) = *dirent;
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
}

void	ft_swap(int *a, int *b)
{
	int c;

	c = *a;
	*a = *b;
	*b = c;
}

void	tabulator_sort_list(t_tab_elem **list, int nb_elem)
{
	t_dirent	**e;
	int		i;
	int		j;

	if (!(e = ft_memalloc(sizeof(t_dirent*) * nb_elem)))
		return ;
	i = -1;
	while (*list && ++i < nb_elem && (e[i] = (*list)->dir))
		*list = (i == nb_elem - 1) ? *list : (*list)->next;
	i = -1;
	while (++i < nb_elem && (j = -1))
		while (++j < nb_elem)
			if (ft_strcmp(e[i]->d_name, e[j]->d_name) < 0)
				ft_swap((int*)&e[i], (int*)&e[j]);
	i = nb_elem;
	while (--i >= 0 && (((*list)->dir = e[i])))
		(*list) = (*list)->prev ? (*list)->prev : *list;
	free(e);
}

void	tabulator_put_color(t_tab_elem *list)
{
	if (list->dir->d_type == 4 || list->dir->d_type == 0)
		ft_putstr(RED);
	else if (list->dir->d_type == 10)
		ft_putstr(BLUE);
	else if (list->dir->d_type == 2 || list->dir->d_type == 6)
		ft_putstr(YEL);
	ft_putstr(list->dir->d_name);
	ft_putstr(END);
}

void	tabulator_put_type(t_tab_elem *list)
{
	if (list->dir->d_type == 2)
		ft_putchar('%');
	else if (list->dir->d_type == 0 || list->dir->d_type == 4)
		ft_putchar('/');
	else if (list->dir->d_type == 6)
		ft_putchar('#');
	else if (list->dir->d_type == 10)
		ft_putchar('@');
	else
		ft_putchar(' ');
}

void	tabulator_put_elem(t_tab_elem *list, int index)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (list && i != index)
	{
		list = list->next;
		i++;
	}
	if (list)
	{
		tabulator_put_color(list);
		tabulator_put_type(list);
	}
}

DIR		*tabulator_open_dir(char **path, char **data)
{
	int len;
	DIR *dir;
	char tmp[ft_strlen(*path) + 1];

	len = ft_strlen(*path);
//	dprintf(2, "path: %s\n", *path);
	if ((dir = opendir(*path)))
		return (dir);
	else
	{
		while (len)
		{
			ft_bzero(tmp, len + 1);
			ft_strncpy(tmp, *path, len);
	//		dprintf(2, "tmp[len]: %c", tmp[len]);
			if ((dir = opendir(tmp)))
			{
				*data = ft_strdup(*path + len);
				ft_strdel(path);
				*path = ft_strdup(tmp);
				return (dir);
			}
			len--;
		}
	}
	return (dir);
}

t_tab	*tabulator_init(t_tab *tabu, int cursor_pos)
{
	if (!(tabu = (t_tab*)malloc(sizeof(t_tab))))
		return (NULL);
	tabu->path = NULL;
	tabu->data = NULL;
	tabu->dir = NULL;
	tabu->elem = NULL;
	tabu->last_elem = NULL;
	tabu->nb_col = 0;
	tabu->nb_row = 0;
	tabu->max_len = 0;
	tabu->nb_node = 0;
	tabu->mode = 0;
	tabu->start = cursor_pos;
	tabu->end = cursor_pos;
	return (tabu);
}

void	check_data_with_space_after(char **new_cmd, char *d_name)
{
	int i;
	int j;
	char *tmp;
	char *tmp1;

	i = -1;
	j = 0;
	tmp = NULL;
	tmp1 = ft_strdup(d_name);
	while (d_name[++i])
	{
		if (d_name[i] == ' ')
		{
			tmp = ft_strsub(tmp1, 0, j);
			tmp = ft_strjoin_free(tmp, "\\");
			tmp = ft_strjoin_free(tmp, d_name + i);
			ft_strdel(&tmp1);
			tmp1 = ft_strdup(tmp);
			ft_strdel(&tmp);
			j++;
		}
		j++;
	}
	*new_cmd = ft_strjoin_free(*new_cmd, tmp1);
	ft_strdel(&tmp1);
}

void	check_data_with_space_before(char **path)
{
	int	i;
	int j;
	char *tmp;
	char *tmp1;

	i = 0;
	j = 0;
	tmp = NULL;
	tmp1 = ft_strdup(*path);
	while ((*path)[i])
	{
		if ((*path)[i] == '\\')
		{
			tmp = ft_strsub(tmp1, 0, j);
			j--;
			tmp = ft_strjoin_free(tmp, *path + i + 1);
			ft_strdel(&tmp1);
			tmp1 = ft_strdup(tmp);
			ft_strdel(&tmp);
		}
		i++;
		j++;
	}
	ft_strdel(path);
	*path = tmp1;
}

void	tabulator_put_new_cmd(t_tab **tabu, t_editor **ed)
{
	char *new_cmd;

	new_cmd = NULL;
	new_cmd = ft_strsub((*ed)->hist->cmd, 0, (*tabu)->start);
//	dprintf(2, "path_here: %s\n", (*tabu)->path);
	(*tabu)->path = ft_strjoin_free((*tabu)->path, (*tabu)->elem->dir->d_name);
//	new_cmd = ft_strjoin_free(new_cmd, (*tabu)->path);
	check_data_with_space_after(&new_cmd, (*tabu)->path);
	if ((*tabu)->elem->dir->d_type == 0 || (*tabu)->elem->dir->d_type == 4)
		new_cmd = ft_strjoin_free(new_cmd, "/");
	new_cmd = ft_strjoin_free(new_cmd, (*ed)->hist->cmd + (*tabu)->end);
	go_to_begin_of_line(*ed);
	ft_putstr("\E[J");
	if ((*ed)->hist->cmd)
		ft_strdel(&(*ed)->hist->cmd);
	(*ed)->hist->cmd = new_cmd;
	ft_putstr(new_cmd);
	(*ed)->cursor_str_pos = ft_strlen(new_cmd);
	(*ed)->cur_col = get_cursor_position(0);
	(*ed)->last_char = (*ed)->cur_col;
}

/*
char	*tabulator_get_path(t_editor *ed, t_tab **tabu)
{
	if ((*tabu)->start && ed->hist->cmd[(*tabu)->start] == ' ' && ed->hist->cmd
	[(*tabu)->start - 1] != ' ' && ed->hist->cmd[(*tabu)->start - 1] != '\\')
		(*tabu)->start--;
	while ((*tabu)->start)
	{
		if (ed->hist->cmd[(*tabu)->start] == ' ' &&
		ed->hist->cmd[(*tabu)->start - 1] != '\\')
		{
			(*tabu)->start++;
			break ;
		}
		(*tabu)->start--;
	}
	(*tabu)->end = (*tabu)->start;
	while (ed->hist->cmd && ed->hist->cmd[(*tabu)->end])
	{
		if (ed->hist->cmd[(*tabu)->end] == ' ' &&
		ed->hist->cmd[(*tabu)->end - 1] != '\\')
			break ;
		(*tabu)->end++;
	}
	return ((*tabu)->end - (*tabu)->start ? ft_strsub(ed->hist->cmd,
	(*tabu)->start, (*tabu)->end - (*tabu)->start)
	: NULL);
}
*/

int		check_if_cmd(t_editor *ed)
{
	int start;

	start = ed->cursor_str_pos ? ed->cursor_str_pos - 1 : 0;
	if (!start && (ed->hist->cmd[start] == '\n' || ed->hist->cmd[start] == ' '
	|| ed->hist->cmd[start] == '\t'))
		return (1);
	else if (!start)
		return (0);
	while (start >= 0)
	{
		if (ed->hist->cmd[start] >= 33 && ed->hist->cmd[start] <= 126)
			return (0);
		start--;
	}
	start = ed->cursor_str_pos ? ed->cursor_str_pos - 1 : 0;
	while (ed->hist->cmd[start])
	{
		if (ed->hist->cmd[start] >= 33 && ed->hist->cmd[start] <= 126)
			return (1);
		start++;
	}
	return (1);
}

int		tabulator_check_cmd(t_editor *ed, t_tab **tabu)
{
	int check;

	(*tabu)->start = ed->cursor_str_pos;
	while (--(*tabu)->start >= -1)
		if ((*tabu)->start == -1 || ed->hist->cmd[(*tabu)->start] == '\n' ||
		ed->hist->cmd[(*tabu)->start] == ' ' || ed->hist->cmd[(*tabu)->start] == '\t')
		{
			(*tabu)->start++;
			break ;
		}
	check = (*tabu)->start;
	while (--check >= 0)
		if (ed->hist->cmd[check] >= 33 && ed->hist->cmd[check] <= 126)
			return (0);
	(*tabu)->end = (*tabu)->start;
	while (ed->hist->cmd[(*tabu)->end])
	{
		if (ed->hist->cmd[(*tabu)->end] == '\n' || ed->hist->cmd[(*tabu)->end]
		== ' ' || ed->hist->cmd[(*tabu)->end] == '\t')
			break ;
		(*tabu)->end++;
	}
	dprintf(2, "(*tabu)->start: %d\n", (*tabu)->start);
	dprintf(2, "(*tabu)->end: %d\n", (*tabu)->end);
	dprintf(2, "cmd[(*tabu)->start]: %c\n", ed->hist->cmd[(*tabu)->start]);
	dprintf(2, "cmd[(*tabu)->end]: %c\n", ed->hist->cmd[(*tabu)->end]);
	dprintf(2, "len: %d\n", (*tabu)->end - (*tabu)->start);
//	dprintf(2, "cmd: [%s]\n", ft_strsub(ed->hist->cmd, (*tabu)->start, (*tabu)->end - (*tabu)->start));
	(*tabu)->data = ft_strsub(ed->hist->cmd, (*tabu)->start, (*tabu)->end - (*tabu)->start);
	return (1);
}

int		tabulator_get_path(t_editor *ed, t_tab **tabu)
{
	if (!(*tabu)->start || ((*tabu)->start && (check_if_cmd(ed) || ed->hist->cmd
	[(*tabu)->start - 1] == ';' || ed->hist->cmd[(*tabu)->start - 1]
	== '&' || ed->hist->cmd[(*tabu)->start - 1] == '|')))
		return (0);
	else if (tabulator_check_cmd(ed, tabu))
		return (1);
//	else if (tabulator_chec_data())
	return (2);
}

void	tabulator_get_binairies(char *b_path, t_tab **tabu)
{
	int		i;
	char	**bin;

	i = -1;
	bin = ft_strsplit(b_path, ':');
	while (bin[++i])
		if (((*tabu)->dir = opendir(bin[i])))
			tabulator_recup_directory_files(tabu);
}

void	term_tabulator(t_editor **ed, char *b_path, e_prompt *prompt)
{
	t_tab *tabu;

	int i;
	int n;
	int ret;

	n = 0;
	tabu = tabulator_init(tabu, (*ed)->cursor_str_pos);
	if (!(tabu->mode = tabulator_get_path(*ed, &tabu)))
	{
		tabulator_get_binairies(b_path, &tabu);
	//	return ;
	}
	else if (tabu->mode == 1)
	{
		tabulator_get_binairies(b_path, &tabu);
		if ((tabu->dir = opendir(".")))
			tabulator_recup_directory_files(&tabu);
		dprintf(2, "YESSS\n");
		return ;
	}
	else 
		return ;
/*	if ((tabu->path = tabulator_get_path(*ed, &tabu)))
	check_data_with_space_before(&tabu->path);
	dprintf(2, "path: %s\n", tabu->path);
	dprintf(2, "path: %s\n", tabu->data);
	tabu->dir = tabulator_open_dir(&tabu->path, &tabu->data);
	if (!tabu->dir)
		return ;
	tabu->max_len = tabulator_recup_directory_files(tabu->dir, &tabu->elem, &tabu->nb_node, tabu->data) + 2;*/
	tabu->nb_col = ((*ed)->ws_col - 2) / tabu->max_len;
	tabu->nb_row = tabu->nb_node / tabu->nb_col;
	dprintf(2, "nb_node: %d\n", tabu->nb_node);
	tabulator_sort_list(&tabu->elem, tabu->nb_node);
	dprintf(2, "nb_row: %d\n", tabu->nb_row);
	if (tabu->nb_row > (*ed)->ws_row)
	{
		ft_putstr("\nDisplay all ");
		ft_putnbr_signed(tabu->nb_node);
		ft_putstr(" possibilities? (y or n)");
		tputs(tgetstr("ve", NULL), 1, ft_putchar);
		while (ret > -1)
		{
			ft_bzero((*ed)->key, BUFF_READ);
			ret = read(0, (*ed)->key, BUFF_READ);
			dprintf(2, "ret: %d\n", ret);
			if (ret == 1 && (*ed)->key[0] == 'y')
				break ;
			else if (ret == 1 && (*ed)->key[0] == 'n')
				return ;
			else if (CTRL_C)
			{
				end_of_text(ed, prompt);
				return ;
			}
		}
	}
	while (tabu->nb_row && (tabu->nb_row * tabu->nb_col) < tabu->nb_node)
		tabu->nb_row++;
	if (tabu->nb_node == 1)
	{
		tabulator_put_new_cmd(&tabu, ed);
		return ;
	}
	ft_putchar('\n');
	i = 0;
	if (!tabu->nb_row && tabu->nb_node > 1)
	{
		while (tabu->elem)
			{
				tabulator_put_color(tabu->elem);
				tabulator_put_type(tabu->elem);
				i++;
				tputs(tgoto(tgetstr("ch", NULL), 0, i * (tabu->max_len + 2)), 1, ft_putchar);
				tabu->elem = tabu->elem->next;
			}
			write(1, "\n", 1);
	}
	while (n < tabu->nb_row)
	{
		if (n + 1 >= (*ed)->ws_row)
		{
			ft_putstr("--More--");
			tputs(tgetstr("ve", NULL), 1, ft_putchar);
				while (1)
				{
					ft_bzero((*ed)->key, BUFF_READ);
					read(STDIN_FILENO, (*ed)->key, BUFF_READ);
					if ((*ed)->key[0] == '\n')
						break ;
					else if (CTRL_C)
					{
						end_of_text(ed, prompt);
						return ;
					}
				}
			tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_putchar);
			ft_putstr("\E[J");
			tabu->nb_node++;
		}
		i = 0;
		if (tabu->nb_row)
		{
			while (i < tabu->nb_col && (n + (i * tabu->nb_row)) <= tabu->nb_node)
			{
				tabulator_put_elem(tabu->elem, n + (i * tabu->nb_row));
				i++;
				tputs(tgoto(tgetstr("ch", NULL), 0, i * (tabu->max_len + 2)), 1, ft_putchar);
				tabu->nb_node++;
			}
		}
		write(1, "\n", 1);
		n++;
	}
	int nb_line;

	nb_line = (*ed)->last_row - (*ed)->first_row;
	if (((*ed)->last_row + tabu->nb_row + nb_line) > (*ed)->ws_row)
	{
		(*ed)->first_row = (*ed)->ws_row - nb_line;
		(*ed)->last_row = (*ed)->ws_row;
	}
	else
	{
		(*ed)->first_row = (*ed)->last_row + tabu->nb_row + 1;
		(*ed)->last_row = (*ed)->first_row + nb_line;
	}
	(*ed)->cursor_str_pos = ft_strlen((*ed)->hist->cmd);
	display_prompt(*prompt);
	ft_putstr((*ed)->hist->cmd);
}
