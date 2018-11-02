/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/05 23:19:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/05 23:19:43 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void	clean_shell(t_shell **shell)
{
	ft_arrdel((*shell)->envp);
	if ((*shell)->str)
		ft_strdel(&(*shell)->str);
	free(*shell);
	*shell = NULL;
}

t_shell	*init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	shell->envp = ft_arrdup(envp);
	shell->envp = rmv_key_env(shell->envp, "OLDPWD");
	shell->mltline = 0;
	shell->str = NULL;
	return (shell);
}

/*
** Retourne le path absolu.
** On commence avec 8 octet puis on monte puissance 2
*/

char	*get_cur_dir(void)
{
	size_t	buf;
	char	*dir;

	buf = 8;
	dir = ft_strnew(buf);
	while (getcwd(dir, buf) == NULL)
	{
		free(dir);
		buf *= 2;
		dir = ft_strnew(buf);
	}
	return (dir);
}
