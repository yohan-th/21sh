/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin_alias.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/04 22:15:59 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/19 16:16:16 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static int		builtin_alias_init(char **args, char **tbl)
{
	int i;

	i = 0;
	tbl[0] = NULL;
	tbl[1] = NULL;
	tbl[2] = NULL;
	if (args[0] && (i = builtin_get_options(&tbl[0], args, "p")) == -1)
	{
		ft_dprintf(2, "42sh: alias: -%s: invalid option\nalias: usage:\
		alias[-p] [name[=value] ... ]\n", tbl[0]);
		ft_strdel(&tbl[0]);
		return (-1);
	}
	return (i);
}

static int		builtin_alias_available(char *arg, char **alias, char *val)
{
	char *cmd;

	if (val && !ft_strncmp(*alias, arg, ft_strlen(arg) - ft_strlen(val)))
	{
		ft_strdel(alias);
		cmd = ft_strsub(arg, 0, ft_strlen(arg) - (ft_strlen(val) + 1));
		*alias = ft_strjoin_mltp(5, cmd, "=", "'", val, "'");
		ft_strdel(&cmd);
		return (1);
	}
	else if (!val && !ft_strncmp(arg, *alias, ft_strlen(arg)))
	{
		ft_dprintf(1, "alias %s\n", *alias);
		return (1);
	}
	return (0);
}

static void		builtin_alias_print(char **alias)
{
	int i;

	i = 0;
	while (alias[i])
		ft_dprintf(1, "alias %s\n", alias[i++]);
}

static void		builtin_alias_not_found(char *arg, char ***alias, char **tb)
{
	char **alias_tmp;

	if (!tb[1])
		ft_dprintf(2, "42sh: alias: %s: not found\n", arg);
	else
	{
		alias_tmp = ft_arrnew(ft_arrlen(*alias) + 1);
		ft_arrcpy(alias_tmp, *alias);
		alias_tmp[ft_arrlen(*alias)] = ft_strjoin_mltp(5, (
		tb[2] = ft_strsub(arg, 0, ft_strlen(arg) -
		(ft_strlen(tb[1]) + 1))), "=", "'", tb[1], "'");
		free(*alias);
		ft_strdel(&tb[2]);
		*alias = alias_tmp;
	}
}

int				builtin_alias(char ***al, char **ag)
{
	int		d[3];
	char	**alias_tmp;
	char	*tb[3];

	if ((d[2] = builtin_alias_init(ag, tb) - 1) == -2)
		return (1);
	if ((d[1] = 1) && ((!ag[0] || (tb[0] && ft_strchr(tb[0], 'p')))))
		builtin_alias_print(*al);
	if ((alias_tmp = *al) && (!tb[0] || ft_strchr(tb[0], 'p')))
		while (ag[++d[2]] && (alias_tmp = *al))
		{
			d[0] = 1;
			tb[1] = ft_strchr(ag[d[2]], '=') ? ft_strchr(ag[d[2]], '=') + 1 : 0;
			while (*alias_tmp)
			{
				if (builtin_alias_available(ag[d[2]],
				&*alias_tmp, tb[1]) && !(d[0] = 0))
					break ;
				alias_tmp++;
			}
			if ((!tb[1] && d[0] && (d[1] = 1)) || (tb[1] && d[0]))
				builtin_alias_not_found(ag[d[2]], al, tb);
		}
	ft_strdel(&tb[0]);
	return (d[1]);
}
