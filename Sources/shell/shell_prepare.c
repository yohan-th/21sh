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

#include "../../Include/shell.h"

int		complete_stdout(t_stdout *std_out, t_shell *shell)
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

/*
** complete_stdout fail si $HOME n'existe pas et ~ est appellé (pw_name vide)
*/

int		shell_prepare_stdout(t_cmd *cmd, t_shell *shell)
{
	t_stdout	*std_out;
	t_cmd		*link;
	int			fd;

	link = cmd;
	while ((link = link->next_cmd))
	{
		std_out = link->std_out;
		while (std_out != NULL)
		{
			complete_stdout(std_out, shell);
			if ((ft_isdir(std_out->to) && write(2, "21sh: ", 6) &&
				 write(2, std_out->to, (size_t)ft_strlen(std_out->to)) &&
				 write(2, ": Is a directory\n", 18)) ||
				((std_out->append && (fd = open(std_out->to, O_WRONLY |
										O_CREAT | O_APPEND, 0644)) < 0) ||
				 (!std_out->append && (fd = open(std_out->to, O_WRONLY |
										O_TRUNC | O_CREAT, 0644)) < 0)))
				return (0);
			close(fd);
			std_out = std_out->next;
		}
	}
	return (1);
}

void	shell_clean_emptyargs(t_cmd *cmd)
{
	char 	**args2;
	t_cmd	*link;
	int 	i;
	int 	n;

	link = cmd;
	while ((link = link->next_cmd))
	{
		args2 = (char **)malloc(sizeof(char *) * (ft_arrlen(link->args) + 1));
		i = 0;
		n = 0;
		while (link->args && link->args[i])
		{
			if (ft_strlen(link->args[i]))
				args2[n++] = ft_strdup(link->args[i]);
			i++;
		}
		args2[n] = NULL;
		ft_arrdel(link->args);
		link->args = args2;
	}
}

void	shell_prepare_args(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*link;
	int 	i;

	link = cmd;
	while ((link = link->next_cmd))
	{
		i = 0;
		while (link->args && link->args[i])
		{
			shl_quotesub(link->args[i]);
			shell_envpsub(&link->args[i], shell->envp);
			if (i == 0)
				link->exec = shell_getpathexec(link->args[0], shell->envp);
			i++;
		}
	}
}

/*
** Clean des arg vide, creation des fichier stdout, get exec
** et bien plus encore !
*/

void	shell_prepare(t_cmd *cmd, t_shell *shell)
{
	shell_prepare_stdout(cmd->start, shell);
	shell_clean_emptyargs(cmd->start);
	shell_prepare_args(cmd, shell);
}