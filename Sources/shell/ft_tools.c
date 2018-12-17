/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_tools.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/11 01:53:07 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 01:53:07 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int		ft_isfile(char *file_path)
{
	struct stat		t_stat;

	if (lstat(file_path, &t_stat) != -1 && S_ISREG(t_stat.st_mode))
		return (1);
	else
		return (0);
}

int 	ft_isdir(char *path)
{
	struct stat		t_stat;

	if (lstat(path, &t_stat) != -1 && S_ISDIR(t_stat.st_mode))
		return (1);
	else
		return (0);
}

