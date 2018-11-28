/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_check.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/28 11:40:36 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/28 12:03:42 by dewalter    ###    #+. /#+    ###.fr     */
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
			tmp = ft_strjoin_free(tmp, "\\");
			tmp = ft_strjoin_free(tmp, d_name + i);
			ft_strdel(&tmp1);
			tmp1 = ft_strdup(tmp);
			ft_strdel(&tmp);
			j++;
		}
		j++;
	}
	*new_cmd = ft_strjoin_free(*new_cmd, tmp1);
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
			tmp = ft_strjoin_free(tmp, *path + i + 1);
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
