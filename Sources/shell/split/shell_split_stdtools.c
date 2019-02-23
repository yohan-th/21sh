/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split_stdtools.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/22 00:38:31 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/22 00:38:31 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

int			len_stdout_to(char *str)
{
	int		i;
	char	quote;

	if (!str)
		return (0);
	quote = ft_strchr("'\"", *str) ? (char)*str : (char)' ';
	i = (quote == ' ') ? 0 : 1;
	while (str[i])
	{
		if (str[i] == '\\' && ft_strlen(str) >= (i + 2) && quote != '\'')
			i += 2;
		if (ft_strchr("'\"", str[i]) && quote == ' ')
			quote = str[i];
		else if (str[i] == quote && quote != ' ')
			quote = ' ';
		if (quote == ' ' && ft_strchr("><", str[i]))
			break ;
		if (str[i] == quote && (quote == ' ' || ft_strchr("\0 ", str[i + 1])))
			break ;
		if (quote == ' ' && str[i] == '-' && i++)
			break ;
		i += (str[i]) ? 1 : 0;
	}
	return (i);
}

t_output	*get_last_stdout(t_output *redi)
{
	t_output	*t_next;

	t_next = redi;
	while (t_next->next)
		t_next = t_next->next;
	return (t_next);
}

/*
** ret se prend un strnew vide pour pouvoir parcourir args[i] lors de son free
*/

char		*complete_stdout_to(char **arg, t_output *add_to)
{
	char *ret;

	add_to->to = ft_strsub(*arg, 0, (size_t)len_stdout_to(*arg));
	if ((ft_strlen(*arg) - len_stdout_to(*arg)) > 0)
		ret = ft_strsub(*arg, (unsigned)len_stdout_to(*arg),
						(size_t)ft_strlen(*arg));
	else
		ret = ft_strnew(1);
	ft_strdel(arg);
	*arg = NULL;
	return (ret);
}

int			len_stdin(char *str, char quote)
{
	int i;

	i = (quote == ' ') ? 0 : 1;
	while (str && str[i])
	{
		if (str[i] == '\\' && ft_strlen(str) >= (i + 2) && quote != '\'')
			i += 2;
		if (ft_strchr("'\"", str[i]) && quote == ' ')
			quote = str[i];
		else if (str[i] == quote && quote != ' ')
			quote = ' ';
		if (quote == ' ' && ft_strchr("><", str[i]))
			break ;
		if (str[i] == quote && (quote == ' ' || ft_strchr("\0 ", str[i + 1])))
			break ;
		i += (str[i] ? 1 : 0);
	}
	return (i);
}

int			shell_stdin_sub(char **arg, int i, char ***std_in)
{
	int		last;
	char	quote;
	int		len;

	last = 0;
	while ((*std_in)[last] != NULL)
		last++;
	if ((*arg)[i] == '\0')
	{
		(*std_in)[last - 1] = (char *)-2;
		return (i);
	}
	quote = ft_strchr("\"'", **arg) ? **arg : (char)' ';
	len = len_stdin(*arg + i, quote);
	if (len > 0)
		(*std_in)[last - 1] = ft_strsub(*arg, (unsigned)i, (size_t)len);
	return (i + len);
}
