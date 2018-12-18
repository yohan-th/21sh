/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/17 21:17:16 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/18 22:51:11 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	calculate_first_and_last_row(t_editor *ed)
{
	int nb_l;

	if ((ed->first_row +
	(nb_l = nb_line(ed->hist->cmd, ed->first_char, ed->ws_col))) > ed->ws_row)
	{
		ed->last_row = ed->ws_row;
		ed->first_row = ed->last_row - nb_l;
	}
	else
		ed->last_row = ed->first_row + nb_l;
}

int		nb_line(char *line, int first_char, int ws_col)
{
	int i;
	int pos;
	int row;

	i = 0;
	pos = first_char;
	row = 0;
	while (line && line[i])
	{
		if (pos == ws_col || line[i] == '\n')
		{
			pos = 0;
			row++;
		}
		i++;
		pos++;
	}
	return (row);
}

void	print_line(char *line, int str_pos, int cur_pos, int ws_col)
{
	while (line && line[str_pos])
	{
		if (line[str_pos] != '\n')
			ft_putchar(line[str_pos]);
		if (line[str_pos] == '\n' || cur_pos == ws_col)
		{
			tputs(tgetstr("sf", NULL), 1, ft_putchar);
			tputs(tgetstr("cr", NULL), 1, ft_putchar);
			cur_pos = 1;
		}
		else
			cur_pos++;
		str_pos++;
	}
}

int		enough_space_on_screen(t_editor *ed)
{
	int new_line_pos;

	new_line_pos = check_if_new_line_in_line(ed);
	if (((ft_strlen(ed->hist->cmd) + ed->prompt_size + ft_strlen(ed->key) - 1))
	>= (ed->ws_col * ed->ws_row) ||
	((ed->first_row == 1 && ed->last_row == ed->ws_row) &&
	((new_line_pos ? new_line_pos : ed->last_char)
	+ ft_strlen(ed->key) - 1 >= ed->ws_col)) ||
	(nb_line(ed->hist->cmd, ed->first_char, ed->ws_col) +
	nb_line(ed->key, ed->last_char, ed->ws_col) > ed->ws_row))
		return (0);
	return (1);
}

int		get_read_key(char **key)
{
	int		ret;
	char	buf[BUFF_READ + 1];

	ft_bzero(buf, BUFF_READ + 1);
	if ((ret = read(STDIN_FILENO, buf, BUFF_READ)))
	{
		buf[ret] = '\0';
		ft_strjoin_free(key, buf);
	}
	if (ret == BUFF_READ)
		get_read_key(key);
	return (ret);
}
