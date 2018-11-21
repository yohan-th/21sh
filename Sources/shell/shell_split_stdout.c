/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split_redi.c                               .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/18 21:05:41 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/18 21:05:41 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int		len_stdout_to(char *str, char quote)
{
	int i;

	i = (quote == ' ') ? 0 : 1;
	while (*str && str[i])
	{
		if (str[i] == '\\' && ft_strlen(str) > (i + 2) && quote == ' ')
			i += 2;
		if (ft_strchr("'\"", str[i]) && quote == ' ')
			quote = str[i];
		else if (str[i] == quote && quote != ' ')
			quote = ' ';
		if (quote == ' ' && ft_strchr("><", str[i]))
			break ;
		if (str[i] == quote && (quote == ' ' || ft_strchr("\0 ", str[i + 1])))
			break ;
		i++;
	}
	return (i);
}

/*
** Return NULL si redi est dans le prochain arg (si existant) ou si quote
** non fermé.
*/

char 	*get_stdout_to(char *redi, int *pos)
{
	char 	*redi_to;
	char 	quote;
	int 	len;

	if (redi[*pos] == '\0') //redi prochain arg
		return (NULL);
	quote = ft_strchr("'\"", redi[*pos]) ? (char)redi[*pos] : (char)' ';
	len = len_stdout_to(redi + *pos, quote);

	if (len > 0)
		redi_to = ft_strsub(redi, (unsigned)*pos, (size_t)len);
	else
		redi_to = NULL;
	*pos += len;
	return (redi_to);
}

int 	get_stdout_from(char *redi, int pos)
{
	char 	*rev;
	char 	*redi_from;
	int 	i;

	if (pos == 0)
		return (1);
	i = 0;
	while (i < pos)
	{
		if (!(ft_isdigit(redi[i++])))
			return (1);
	}
	rev = ft_strrev(ft_strdup(redi));
	pos = ft_strlen(redi) - pos;
	redi_from = ft_strsub(rev, (unsigned)pos, (size_t)ft_strlen(rev) - pos);
	ft_strdel(&rev);
	i = ft_atoi(ft_strrev(redi_from));
	ft_strdel(&redi_from);
	return (i);
}

/*
** tronque {arg} si {from} n'a pas que des digits
*/

int 	shell_stdout_sub(char **arg, int i, t_stdout *redi)
{
	redi->from = get_stdout_from(*arg, i);
	ft_strdel(&redi->to);
	if ((*arg)[i] && (*arg)[i + 1] == '>')
	{
		redi->append = 1;
		(*arg)[i] = '\0';
		i += 2;
		redi->to = get_stdout_to(*arg, &i);
	}
	else if ((*arg)[i])
	{
		redi->append = 0;
		(*arg)[i] = '\0';
		i += 1;
		redi->to = get_stdout_to(*arg, &i);
	}
	if (*arg && ft_atoi(*arg) == redi->from)
		*arg[0] = '\0';
	return (i);
}

t_stdout	*add_stdout(t_stdout **first_redi)
{
	t_stdout	*new_redi;
	t_stdout	*t_next;

	if (!(new_redi = malloc(sizeof(t_stdout))))
		return (NULL);
	new_redi->append = 0;
	new_redi->from = 1;
	new_redi->to = ft_strdup("&1");
	new_redi->next = NULL;
	if (!(*first_redi))
	{
		new_redi->start = new_redi;
		*first_redi = new_redi;
	}
	else
	{
		new_redi->start = *first_redi;
		t_next = *first_redi;
		while (t_next->next != NULL)
			t_next = t_next->next;
		t_next->next = new_redi;
	}
	return (new_redi);
}

t_stdout		*get_last_stdout(t_stdout *redi)
{
	t_stdout	*t_next;

	t_next = redi;
	while (t_next->next)
		t_next = t_next->next;
	return (t_next);
}

char		*complete_stdout_to(char **arg, t_stdout *add_to, char quote)
{
	char *ret;

	add_to->to = ft_strsub(*arg, 0, (size_t) len_stdout_to(*arg, quote));
	if ((ft_strlen(*arg) - len_stdout_to(*arg, quote)) > 0)
		ret = ft_strsub(*arg, (unsigned) len_stdout_to(*arg, quote),
						(size_t )ft_strlen(*arg));
	else
		ret = NULL;
	ft_strdel(arg);
	*arg = NULL;
	return (ret);
}

/*
** shell redi peut renvoyer NULL si malloc fail.
** On effectue un exit propre (à faire).
*/

t_stdout		*shell_std_out(char **arg, t_stdout **first_redi, char quote)
{
	int		i;
	t_stdout	*redi;

	if (*first_redi && (get_last_stdout(*first_redi))->to == NULL && *arg)
		*arg = complete_stdout_to(arg, get_last_stdout(*first_redi), quote);
	redi = NULL;
	i = (quote == ' ') ? 0 : 1;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2) && quote == ' ')
			i += 2;
		if (ft_strchr("'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '>' && quote == ' ')
		{
			if (!(redi = add_stdout(first_redi)))
				return (NULL);
			i = shell_stdout_sub(arg, i, redi);
		}
		else
			i++;
	}
	return (redi);
}
