/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   term_history.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/31 20:23:29 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/03 15:00:55 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	term_history_up(t_editor **ed, char *line)
{
	t_history *tmp;

	tmp = NULL;
	if (!(*ed)->hist->next)
	{
		if (ft_strlen(line) && !ft_strcmp((*ed)->hist->cmd, line))
			(*ed)->hist = (*ed)->hist->prev;
	}
	else
	{
		if (!(*ed)->hist->prev)
			tmp = (*ed)->hist;
		(*ed)->hist = tmp ? tmp : (*ed)->hist->prev;
	}
}

void	term_history_down(t_editor **ed)
{
	t_history *tmp;

	tmp = NULL;
	if (!(*ed)->hist->next)
		tmp = (*ed)->hist;
	(*ed)->hist = tmp ? tmp : (*ed)->hist->next;
}

void	term_history_put_cmd(t_editor **ed)
{
	int nb_l;

	go_to_begin_of_line(*ed);
	ft_putstr("\E[J");
	nb_l = nb_line(*ed);
	ft_putstr((*ed)->hist->cmd);
	(*ed)->cursor_str_pos = ft_strlen((*ed)->hist->cmd);
	(*ed)->last_char = get_cursor_position(0);
	if ((*ed)->last_char == (*ed)->ws_col)
	{
		tputs(tgetstr("do", NULL), 1, ft_putchar);
		(*ed)->last_char = 1;
	}
	(*ed)->last_row = get_cursor_position(1);
	(*ed)->cur_row = (*ed)->last_row;
	(*ed)->first_row = (*ed)->last_row - nb_l;
	(*ed)->cur_col = (*ed)->last_char;
}

void	term_history(t_editor **ed)
{
	if (UP_KEY)
	{
		if ((*ed)->hist->prev)
		{
			(*ed)->hist = (*ed)->hist->prev;
			term_history_put_cmd(ed);
		}
	}
	else
	{
		if ((*ed)->hist->next)
		{
			(*ed)->hist = (*ed)->hist->next;
			term_history_put_cmd(ed);
		}
	}
}
