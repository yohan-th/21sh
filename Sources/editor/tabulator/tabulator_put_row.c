/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_put_row.c                              .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/28 11:32:28 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/17 21:45:52 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	tabulator_put_color(t_tab_elem *list)
{
	int i;

	i = -1;
	if (list->d_type == 4)
		ft_putstr(RED);
	else if (list->d_type == 10)
		ft_putstr(BLUE);
	else if (list->d_type == 2 || list->d_type == 6)
		ft_putstr(YEL);
	else if (list->d_type == 16)
		ft_putstr("\033[0;49;92m");
	if (check_if_name_with_new_line(list->d_name))
	{
		while (list->d_name[++i])
			write(1, list->d_name[i] == '\n' || list->d_name[i] == '\t' ?
			"?" : &(list->d_name)[i], 1);
	}
	else
		ft_putstr(list->d_name);
	ft_putstr(END);
}

void	tabulator_put_type(t_tab_elem *list)
{
	if (list->d_type == 2)
		ft_putchar('%');
	else if (list->d_type == 4)
		ft_putchar('/');
	else if (list->d_type == 6)
		ft_putchar('#');
	else if (list->d_type == 10)
		ft_putchar('@');
	else if (list->d_type == 16)
		ft_putchar('*');
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

void	tabulator_one_row(t_tab_elem *elem, int max_len)
{
	int i;

	i = 0;
	while (elem)
	{
		tabulator_put_color(elem);
		tabulator_put_type(elem);
		i++;
		tputs(tgoto(tgetstr("ch", NULL), 0, i *
		(max_len + 2)), 1, ft_putchar);
		elem = elem->next;
	}
	write(1, "\n", 1);
}

int		tabulator_multi_row(t_tab *tabu, t_editor *ed)
{
	int i;
	int n;
	int ret;

	if (tabu->nb_row > (int)ed->ws_row &&
	((!(ret = tabulator_read(tabu, ed, 0)) || ret == -1 || ret == -3)))
		return (ret);
	n = -1;
	while (++n < tabu->nb_row && !(i = 0))
	{
		if (n + 1 >= ed->ws_row &&
		(((ret = tabulator_read(tabu, ed, 1)) == -1 || ret == -3)))
			return (ret);
		while (tabu->nb_row && i < tabu->nb_col
		&& (n + (i * tabu->nb_row)) <= tabu->nb_node)
		{
			tabulator_put_elem(tabu->elem, n + (i++ * tabu->nb_row));
			tputs(tgoto(tgetstr("ch", NULL), 0, i *
			(tabu->max_len + 2)), 1, ft_putchar);
			tabu->nb_node++;
		}
		write(1, "\n", 1);
	}
	return (1);
}
