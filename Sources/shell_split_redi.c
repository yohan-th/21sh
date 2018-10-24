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

#include "../Include/shell.h"

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
** Return NULL si redi est dans le prochain arg (si existant) ou si quote non
** non fermé.
*/

char 	*get_redi_to(char *redi, int pos)
{
	char 	*redi_to;
	char 	quote;
	int 	len;

	if (redi[pos] == '\0') //redi prochain arg
		return (NULL);
	quote = ft_strchr("'\"", redi[pos]) ? (char)redi[pos] : (char)'\0';
	len = ft_strlen(redi) - pos;
	if (quote != '\0')
		redi_to = ft_strsub(redi, (unsigned)pos + 1, (size_t)len - 2);
	else
		redi_to = ft_strsub(redi, (unsigned)pos, (size_t)len);
	return (redi_to);
}

/*
** tronque arg si from n'a pas que des digits
*/

void 	shell_redi_sub(char **arg, int i, t_redi *redi)
{
	redi->from = get_redi_from(*arg, i);
	ft_strdel(&redi->to);
	if ((*arg)[i + 1] != '>')
	{
		redi->append = 0;
		redi->to = get_redi_to(*arg, i + 1);
	}
	else
	{
		redi->append = 1;
		redi->to = get_redi_to(*arg, i + 2);
	}
	(*arg)[i] = '\0';
	if (ft_atoi(*arg) == redi->from)
		*arg[0] = '\0';
}

t_redi	*add_redi(t_args *first_redi)
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
	if (!(first_redi->redi))
	{
		new_redi->start = new_redi;
		first_redi->redi = new_redi;
	}
	else
	{
		new_redi->start = first_redi->redi;
		t_next = first_redi->redi;
		while (t_next->next != NULL)
			t_next = t_next->next;
		t_next->next = new_redi;
	}
	return (new_redi);
}

/*
** shell redi peut renvoyer 0 si malloc fail. On effectue alors un exit propre
*/

int		shell_redi(char **arg, t_args *st_args, char quote)
{
	int		i;
	t_redi	*redi;

	i = (quote == ' ') ? 0 : 1;
	while ((*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2))
			i += 2;
		if (ft_strchr("'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '>' && quote == ' ')
		{
			if (!(redi = add_redi(st_args)))
				return (0);
			shell_redi_sub(arg, i, redi);
			break ;
		}
		i++;
	}
	return (1);
}
