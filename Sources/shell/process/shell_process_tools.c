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

#include "../../../Include/shell.h"

/*
** Ajoute la dossier courant
*/

int		complete_stdout_path(t_stdout *std_out, t_shell *shell)
{
	char 		*tmp;
	t_stdout 	*link;
	char 		*cur_dir;

	link = std_out;
	while (link != NULL)
	{
		shell_envpsub(&link->to, shell->envp);
		shl_quotesub(link->to);
		if (link->to[0] != '/' && link->to[0] != '&')
		{
			cur_dir = get_cur_dir();
			tmp = link->to;
			link->to = ft_strjoin_mltp(3, cur_dir, "/", link->to);
			ft_strdel(&tmp);
			ft_strdel(&cur_dir);
		}
		link = link->next;
	}
	return (1);
}

//)

void 	complete_stdin_path(t_cmd *link, t_shell *shell)
{
	int		i;
	char 	*tmp;
	char 	*std_in;
	char 	*cur_dir;

	i = 0;
	while (link->std_in && link->std_in[i])
	{
		std_in = link->std_in[i];
		shell_envpsub(&std_in, shell->envp);
		shl_quotesub(std_in);
		if (std_in[0] != '/' && std_in[0] != '&')
		{
			tmp = std_in;
			cur_dir = get_cur_dir();
			std_in = ft_strjoin_mltp(3, cur_dir, "/", std_in);
			ft_strdel(&cur_dir);
			ft_strdel(&tmp);
		}
		i++;
	}
}
