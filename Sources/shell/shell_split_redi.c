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

int		len_redi_to(char *str, char quote)
{
	int i;

	i = (quote == ' ') ? 0 : 1;
	while (*str && str[i])
	{
		if (str[i] == '\\' && ft_strlen(str) > (i + 2))
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

char 	*get_redi_to(char *redi, int *pos)
{
	char 	*redi_to;
	char 	quote;
	int 	len;

	if (redi[*pos] == '\0') //redi prochain arg
		return (NULL);
	quote = ft_strchr("'\"", redi[*pos]) ? (char)redi[*pos] : (char)' ';
	len = len_redi_to(redi + *pos, quote);

	if (len > 0)
		redi_to = ft_strsub(redi, (unsigned)*pos, (size_t)len);
	else
		redi_to = NULL;
	*pos += len;
	return (redi_to);
}

int 	get_redi_from(char *redi, int pos)
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
	redi_from = ft_strsub(rev, (unsigned int)pos, (size_t)ft_strlen(rev) - pos);
	ft_strdel(&rev);
	i = ft_atoi(ft_strrev(redi_from));
	ft_strdel(&redi_from);
	return (i);
}

/*
** tronque {arg} si {from} n'a pas que des digits
*/

int 	shell_redi_sub(char **arg, int i, t_redi *redi)
{
	redi->from = get_redi_from(*arg, i);
	ft_strdel(&redi->to);
	if ((*arg)[i] && (*arg)[i + 1] == '>')
	{
		redi->append = 1;
		(*arg)[i] = '\0';
		i += 2;
		redi->to = get_redi_to(*arg, &i);
	}
	else if ((*arg)[i])
	{
		redi->append = 0;
		(*arg)[i] = '\0';
		i += 1;
		redi->to = get_redi_to(*arg, &i);
	}
	if (*arg && ft_atoi(*arg) == redi->from)
		*arg[0] = '\0';
	return (i);
}

t_redi	*add_redi(t_redi **first_redi)
{
	t_redi	*new_redi;
	t_redi	*t_next;

	if (!(new_redi = malloc(sizeof(t_redi))))
		return (NULL);
	new_redi->append = 0;
	new_redi->from = 1;
	new_redi->to = ft_strdup("&1");
	new_redi->std_in = NULL;
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

t_redi		*get_last_redi(t_redi *redi)
{
	t_redi	*t_next;

	t_next = redi;
	while (t_next->next)
		t_next = t_next->next;
	return (t_next);
}

char		*complete_redi_to(char **arg, t_redi *add_to, char quote)
{
	char *ret;

	add_to->to = ft_strsub(*arg, 0, (size_t)len_redi_to(*arg, quote));
	if ((ft_strlen(*arg) - len_redi_to(*arg, quote)) > 0)
		ret = ft_strsub(*arg, (unsigned)len_redi_to(*arg, quote),
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

t_redi		*shell_redi(char **arg, t_redi **first_redi, char quote)
{
	int		i;
	t_redi	*redi;

	if (*first_redi && (get_last_redi(*first_redi))->to == NULL && *arg)
		*arg = complete_redi_to(arg, get_last_redi(*first_redi), quote);
	redi = NULL;
	i = (quote == ' ') ? 0 : 1;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2))
			i += 2;
		if (ft_strchr("'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '>' && quote == ' ')
		{
			if (!(redi = add_redi(first_redi)))
				return (NULL);
			i = shell_redi_sub(arg, i, redi);
		}
		else
			i++;
	}
	return (redi);
}
