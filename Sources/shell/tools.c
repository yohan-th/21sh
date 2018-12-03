/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/05 23:19:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/03 14:32:09 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int 	clean_data(t_cmd *cmd, t_shell *shell, BOOL t_cmd, BOOL shl_str)
{
	if (t_cmd)
		clean_cmd(&cmd);
	if (shl_str)
		ft_strdel(&shell->str);
	return (1);
}

/*
** Lorsqu'on lance shell, OLDPWD ne doit pas exister, on le del de dup_envp
*/

t_shell		*init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	shell->envp = ft_arrdup(envp);
	shell->envp = rmv_key_env(shell->envp, "OLDPWD");
	shell->str = NULL;
	shell->hist = init_hist();
	return (shell);
}

/*
** Retourne le path absolu.
** On commence avec 8 octet puis on monte puissance 2
*/

char		*get_cur_dir(void)
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
