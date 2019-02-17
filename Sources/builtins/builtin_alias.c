/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin_alias.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/04 22:15:59 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/17 14:40:06 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int 	builtin_alias_init(char **args, char **op, int *i, BOOL v)
{
	if (args[0])
		*i = builtin_get_options(op, args, "p") - 1;
	if (args[0] && *i == -2)
	{
		if (!v)
		{
			ft_dprintf(2, "42sh: alias: -%s: invalid option\nalias: usage:\
			alias[-p] [name[=value] ... ]\n", op);
		}
		ft_strdel(op);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int 	builtin_alias(t_data **alias, char **args, BOOL v)
{
	int i;
	int match[2];
	char *op;
	char *val;
	t_data *tmp;
	char *cmd;

	op = NULL;
	match[0] = 0;
	match[1] = 0;
	dprintf(1, "match: %d\n", match[0]);
	dprintf(1, "match: %d\n", match[1]);
	if (builtin_alias_init(args, &op, &i, v) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	tmp = (*alias);
	if ((!args[0] || (op && ft_strchr(op, 'p'))))
	{
		while ((*alias)->next)
		{
			dprintf(1, "(*alias): %s\n", (*alias)->cmd);
			(*alias) = (*alias)->next;
		}
	}
	if (!op || ft_strchr(op, 'p'))
	{
		while (args[++i] && ((*alias) = tmp))
		{
			val = ft_strchr(args[i], '=');
			dprintf(1, "val: %s\n", val);
			while (((!v && !val) || (v && val)) && (*alias)->next)
			{
				if (val && !ft_strncmp((*alias)->cmd, args[i], ft_strlen(args[i]) - ft_strlen(val)))
				{
					dprintf(1, "cmd: %s\n", (*alias)->cmd);
					ft_strdel(&(*alias)->cmd);
					dprintf(1, "cmd_null: %s\n", (*alias)->cmd);
					cmd = ft_strsub(args[i], 0, ft_strlen(args[i]) - ft_strlen(val));
					(*alias)->cmd = ft_strjoin_mltp(3, cmd, "=", val + 1);
					dprintf(1, "cmd: %s\n", (*alias)->cmd);
				}
				else if (!val && !ft_strncmp(args[i], (*alias)->cmd, ft_strlen(args[i])))
					dprintf(1, "(*alias)_here: %s\n", (*alias)->cmd);
				(*alias) = (*alias)->next;
			}
		}
	}
	(*alias) = tmp;
	/*while ((*(*alias))->prev)
		*alias = (*alias)->prev;*/
	/*while ((*alias)->next)
	{
		dprintf(1, "alias0: %s\n", (*alias)->cmd);
		(*alias) = (*alias)->next;
	}*/
	dprintf(1, "alias_alias: %p\n", alias);
	return (EXIT_SUCCESS);
}
