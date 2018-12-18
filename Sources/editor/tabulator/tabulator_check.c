/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_check.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/06 16:42:37 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/17 20:52:06 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	check_data_with_space_after(char **new_cmd, char *d_name)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp1;

	i = -1;
	j = 0;
	tmp = NULL;
	tmp1 = ft_strdup(d_name);
	while (d_name[++i])
	{
		if (d_name[i] == ' ')
		{
			tmp = ft_strsub(tmp1, 0, j);
			ft_strjoin_free(&tmp, "\\");
			ft_strjoin_free(&tmp, d_name + i);
			ft_strdel(&tmp1);
			tmp1 = ft_strdup(tmp);
			ft_strdel(&tmp);
			j++;
		}
		j++;
	}
	ft_strjoin_free(new_cmd, tmp1);
	ft_strdel(&tmp1);
}

void	check_data_with_space_before(char **path)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp1;

	i = 0;
	j = 0;
	tmp = NULL;
	tmp1 = ft_strdup(*path);
	while ((*path)[i])
	{
		if ((*path)[i] == '\\')
		{
			tmp = ft_strsub(tmp1, 0, j);
			j--;
			ft_strjoin_free(&tmp, *path + i + 1);
			ft_strdel(&tmp1);
			tmp1 = ft_strdup(tmp);
			ft_strdel(&tmp);
		}
		i++;
		j++;
	}
	ft_strdel(path);
	*path = tmp1;
}

int		check_if_name_with_new_line(char *name)
{
	int i;

	i = -1;
	while (name[++i])
		if (name[i] == '\n')
			return (1);
	return (0);
}
