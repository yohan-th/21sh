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

void	shell_clean_emptyargs(t_cmd *elem)
{
	char	**args2;
	int		i;
	int		n;

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
	ft_arrdel(&elem->args);
	elem->args = args2;
}

void	shell_prepare_args(t_cmd *elem, t_shell *shell)
{
	int i;

	i = 0;
	while (elem->args && elem->args[i])
	{
		shell_envpsub(&elem->args[i], shell->envp);
		shl_quotesub(elem->args[i]);
		if (i == 0)
		{
			elem->exec = shell_getpathexec(elem->args[0], shell->envp);
			if (!elem->exec)
				elem->exec = ft_strdup("not found");
		}
		i++;
	}
}

void	shell_clean_input(t_cmd *elem)
{
	int		i;
	char	**files;
	int		n;

	if (elem->input)
	{
		files = malloc(sizeof(char *) * (ft_arrlen(elem->input) + 1));
		i = 0;
		n = 0;
		while (elem->input && elem->input[i])
		{
			if ((int)elem->input[i] != -1 && (int)elem->input[i] != -3)
			{
				files[n++] = ft_strdup(elem->input[i]);
				ft_strdel(&elem->input[i]);
			}
			i++;
		}
		if (n == 0)
			ft_arrdel(&files);
		else
			files[n] = NULL;
		free(elem->input);
		elem->input = files;
	}
}

/*
** Clean des arg vide et get exec path et clean les inputs à -1 de HRDC
*/

void	shell_prepare(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*elem;

	elem = cmd;
	while ((elem = elem->next_cmd))
	{
		shell_clean_emptyargs(elem);
		shell_prepare_args(elem, shell);
	}
}
