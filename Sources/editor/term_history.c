/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   term_history.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/31 20:23:29 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/07 18:19:04 by dewalter    ###    #+. /#+    ###.fr     */
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

void	hist_position(t_editor **ed)
{
	int nb;
	int i;

	nb = 0;
	i = 0;
	while ((*ed)->hist->cmd && (*ed)->hist->cmd[i])
	{
		if (((i + (*ed)->first_char) == (*ed)->ws_col) ||
		((*ed)->hist->cmd[i] == '\n'))
			nb++;
		i++;
	}
	if (((*ed)->first_row + nb) > (*ed)->ws_row)
	{
		(*ed)->first_row = (*ed)->first_row -
		(((*ed)->first_row + nb) - (*ed)->ws_row);
		(*ed)->last_row = (*ed)->ws_row;
	}
	else
		(*ed)->last_row = (*ed)->first_row + nb;
	(*ed)->cur_row = (*ed)->last_row;
}

void	term_history(t_editor **ed)
{
	if (UP_KEY)
	{
		if ((*ed)->hist->prev)
		{
			(*ed)->hist = (*ed)->hist->prev;
		}
	}
	else
	{
		if ((*ed)->hist->next)
		{
			(*ed)->hist = (*ed)->hist->next;
		}
	}
	go_to_begin_of_line(*ed);
	ft_putstr("\E[J");
	ft_putstr((*ed)->hist->cmd);
	(*ed)->cursor_str_pos = ft_strlen((*ed)->hist->cmd);
	hist_position(ed);
	(*ed)->cur_col = get_cursor_position(0);
	(*ed)->last_char = (*ed)->cur_col;








	/*
	if ((*ed)->hist->cmd)
		ft_strcpy(line, (*ed)->hist->cmd);
	if (UP_KEY)
	{
		if (!(*ed)->hist->prev && !ft_strcmp((*ed)->hist->cmd, line))
			return ;
		term_history_up(ed, line);
	}
	else if (DOWN_KEY)
	{
		if (!(*ed)->hist->next)
			return ;
		term_history_down(ed);
	}
	go_to_begin_of_line(*ed);
	ft_putstr("\E[J");
	ft_putstr((*ed)->hist->cmd);
	(*ed)->cursor_str_pos = ft_strlen((*ed)->hist->cmd);
	hist_position(ed);
	(*ed)->cur_col = get_cursor_position(0);
	(*ed)->last_char = (*ed)->cur_col;*/
}
