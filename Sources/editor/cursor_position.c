/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cursor_position.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/12 18:13:31 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/24 21:04:53 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

char			*cursor_position_escape_sequence(int row, int col, t_editor *ed)
{
	char *h_pos;
	char *v_pos;
	char *cursor_pos;

	v_pos = ft_itoa(ed->cur_row - row);
	h_pos = ft_itoa(ed->cur_col - col);
	cursor_pos = ft_strnew(6 + ft_strlen(v_pos) + ft_strlen(h_pos));
	ft_strcpy(cursor_pos, "\E[");
	ft_strcat(cursor_pos, v_pos);
	ft_strcat(cursor_pos, ";");
	ft_strcat(cursor_pos, h_pos);
	ft_strcat(cursor_pos, "H");
	ft_strdel(&h_pos);
	ft_strdel(&v_pos);
	return (cursor_pos);
}

void			reset_cursor_position_escape_sequence(char **cursor_positon)
{
	if (cursor_positon)
	{
		ft_putstr(*cursor_positon);
		ft_strdel(cursor_positon);
	}
}

static int		get_row(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			break ;
		i++;
	}
	return (ft_atoi(str + i));
}

static int		get_col(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (i && str[i - 1] == ';' && str[i] >= '0' && str[i] <= '9')
			break ;
	return (ft_atoi(str + i));
}

size_t			get_cursor_position(int mode)
{
	size_t	res;
	char	buf[32];
	unsigned int i;

	i = 0;
	if (write(1, "\x1b[6n", 4) != 4)
		return (-1);
	while (i < sizeof(buf) -1)
	{
		if (read(0,buf+i,1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';
	if (buf[0] != 27 || buf[1] != '[')
		return (-1);
	if (mode)
		res = get_row(buf);
	else
		res = get_col(buf);
	return (res);
}
