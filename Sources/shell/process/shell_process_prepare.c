/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_prepare.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/12 01:08:34 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/12 01:08:34 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

void	shell_clean_emptyargs(t_cmd *cmd)
{
	char 	**args2;
	t_cmd	*elem;
	int 	i;
	int 	n;

	elem = cmd;
	while ((elem = elem->next_cmd))
	{
		args2 = (char **)malloc(sizeof(char *) * (ft_arrlen(elem->args) + 1));
		i = 0;
		n = 0;
		while (elem->args && elem->args[i])
		{
			if (ft_strlen(elem->args[i]))
				args2[n++] = ft_strdup(elem->args[i]);
			i++;
		}
		args2[n] = NULL;
		ft_arrdel(elem->args);
		elem->args = args2;
	}
}

void	shell_prepare_args(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*elem;
	int 	i;

	elem = cmd;
	while ((elem = elem->next_cmd))
	{
		i = 0;
		while (elem->args && elem->args[i])
		{
			shl_quotesub(elem->args[i]);
			shell_envpsub(&elem->args[i], shell->envp);
			if (i == 0)
				elem->exec = shell_getpathexec(elem->args[0], shell->envp);
			i++;
		}
	}
}

/*
** Clean des arg vide get exec path
*/

void	shell_prepare(t_cmd *cmd, t_shell *shell)
{
	shell_clean_emptyargs(cmd);
	shell_prepare_args(cmd, shell);
}
