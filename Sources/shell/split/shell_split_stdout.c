/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split_stdout.c                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/22 00:38:38 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/22 00:38:38 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

/*
** Return NULL si redi est dans le prochain arg (si existant) ou si quote
** non fermé.
*/

char		*get_stdout_to(char *redi, int *pos)
{
	char	*redi_to;
	char	quote;
	int		len;

	if (redi[*pos] == '\0')
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

int			get_stdout_from(char *redi, int pos)
{
	char	*rev;
	char	*redi_from;
	int		i;

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

int			shell_stdout_sub(char **arg, int i, t_output *redi)
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

t_output	*add_stdout(t_output **first_redi)
{
	t_output	*new_redi;
	t_output	*t_next;

	if (!(new_redi = malloc(sizeof(t_output))))
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

/*
** shell redi peut renvoyer NULL si malloc fail.
** On effectue un exit propre (à faire).
*/

t_output	*shell_std_out(char **arg, t_output **first_redi, char quote)
{
	int			i;
	t_output	*redi;

	if (*first_redi && (get_last_stdout(*first_redi))->to == NULL && *arg)
		*arg = complete_stdout_to(arg, get_last_stdout(*first_redi), quote);
	redi = NULL;
	i = (quote == ' ') ? 0 : 1;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) >= (i + 2) && quote != '\'')
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
			i += ((*arg)[i]) ? 1 : 0;
	}
	return (redi);
}
