/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process_tools.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/11 01:01:54 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 01:01:54 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

/*
** Ajoute la dossier courant
*/

int		complete_stdout_path(t_stdout *std_out, t_shell *shell)
{
	char 		*tmp;
	t_stdout 	*link;

	link = std_out;
	while (link != NULL)
	{
		shell_envpsub(&link->to, shell->envp);
		shl_quotesub(link->to);
		if (link->to[0] != '/' && link->to[0] != '&')
		{
			tmp = link->to;
			link->to = ft_strjoin_mltp(3, shell->cur_dir_run, "/", link->to);
			ft_strdel(&tmp);
		}
		link = link->next;
	}
	return (1);
}

void 	complete_stdin_path(char **std_in, t_shell *shell)
{
	int		i;
	char 	*tmp;

	i = 0;
	while (std_in && std_in[i])
	{
		shell_envpsub(&std_in[i], shell->envp);
		shl_quotesub(std_in[i]);
		if (std_in[i][0] != '/' && std_in[i][0] != '&')
		{
			tmp = std_in[i];
			std_in[i] = ft_strjoin_mltp(3, shell->cur_dir_run, "/", std_in[i]);
			ft_strdel(&tmp);
		}
		i++;
	}
}
