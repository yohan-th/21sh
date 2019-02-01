/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin_type_tools.c                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/01 23:21:54 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/01 23:21:54 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int		builtin_type_get_options(char **options, char **args)
{
	int i;
	int j;

	i = 0;
	while (args[i] && args[i][0] == '-' && args[i][1] && !(j = 0))
	{
		if (args[i][1] == '-' && args[i][2] == '\0')
			break ;
		while (args[i][++j])
			if (ft_strchr("afptP", args[i][j]))
			{
				if (!*options || !ft_strchr(*options, args[i][j]))
					ft_char_join_free(args[i][j], options);
			}
			else
			{
				ft_strdel(options);
				ft_char_join_free(args[i][j], options);
				return (-1);
			}
		i++;
	}
	return (i);
}

int		builtin_type_check_builtin(char *d_name)
{
	if (!ft_strcmp(d_name, "echo") || !ft_strcmp(d_name, "cd") ||
		!ft_strcmp(d_name, "type") || !ft_strcmp(d_name, "unsetenv") ||
		!ft_strcmp(d_name, "setenv") || !ft_strcmp(d_name, "exit") ||
		!ft_strcmp(d_name, "env"))
		return (1);
	return (0);
}

void	builtin_type_display(char *d_name, char *bin, char *options, int mode)
{
	if (!mode)
	{
		if (options && ft_strchr(options, 't'))
			ft_putstr("builtin\n");
		else
			ft_dprintf(1, "%s is a shell builtin\n", d_name);
	}
	else
	{
		if (!options || !ft_strchr(options, 't'))
		{
			if (!options || !ft_strchr(options, 'P'))
				ft_dprintf(1, "%s is ", d_name);
			if (bin)
				ft_dprintf(1, "%s/%s\n", bin, d_name);
			else
				ft_putendl(d_name);
		}
		else
			ft_putstr("file\n");
	}
}

int		check_executable_file(char *path)
{
	struct stat stat;

	lstat(path, &stat);
	if (S_ISREG(stat.st_mode) && stat.st_mode & S_IXUSR)
		return (1);
	return (0);
}
