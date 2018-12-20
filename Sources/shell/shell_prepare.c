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
			complete_stdout_path(std_out, shell);
			if (ft_isdir(std_out->to))
				return (shell_error_prepare("Is directory", std_out->to));
			else if ((std_out->append && (fd = open(std_out->to, O_WRONLY |
							O_CREAT | O_APPEND, 0644)) < 0 && close(fd)) ||
					((!std_out->append && (fd = open(std_out->to, O_WRONLY |
							O_TRUNC | O_CREAT, 0644)) < 0) && close(fd)))
				return (shell_error_prepare("denied", std_out->to));
			else
				std_out = std_out->next;
		}
	}
	return (1);
}

int 	shell_prepare_stdin(t_cmd *cmd, t_shell *shell)
{
	t_cmd		*link;
	int			fd;
	int 		i;

	link = cmd;
	while ((link = link->next_cmd))
	{
		complete_stdin_path(link->std_in, shell);
		i = 0;
		while (link->std_in && link->std_in[i])
		{
			if (ft_isdir(link->std_in[i]))
				return (shell_error_prepare("Is directory", link->std_in[i]));
			else if ((fd = open(link->std_in[i], O_RDONLY, 0644) < 0))
				return (shell_error_prepare("denied", link->std_in[i]));
			else
			{
				close(fd);
				i++;
			}
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

int		shell_prepare(t_cmd *cmd, t_shell *shell)
{
	shell_clean_emptyargs(cmd);
	shell_prepare_args(cmd, shell);
	if (!shell_prepare_stdin(cmd, shell) || !shell_prepare_stdout(cmd, shell))
		return (0);
	else
		return (1);
}
