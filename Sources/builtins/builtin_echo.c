/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin_echo.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/07 19:56:27 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/24 22:13:49 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"

void	builtin_echo(char **cmd)
{
	int		i;
	BOOL	newl;

	newl = 1;
	i = 1;
	if (ft_strcmp(cmd[1], "-e") == 0)
	{
		newl = 0;
		i = 2;
	}
	while (cmd[i] != NULL || cmd[i])
	{
		if (ft_strcmp(cmd[i], "") != 0)
			ft_printf("%s ", cmd[i]);
		i++;
	}
	if (newl)
		write(1, "\n", 1);
}
