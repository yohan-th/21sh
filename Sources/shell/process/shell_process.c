/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/28 20:39:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/06/28 20:39:57 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

/*
** ATTENTION shell->str peut etre vide apres un heredoc
*/

#include "../../../Include/shell.h"

int		shell_process(t_cmd **cmd, t_shell *shell)
{
	t_cmd	*elem;
	int 	fd[3];
	int		tmp_fd[3];
	int		built_in;

	shell_prepare(*cmd, shell);
	shell_save_fd(fd);
	built_in = 0;
	signal(SIGINT, shell_prcs_sigint);
	elem = *cmd;
	while ((elem = elem->next_cmd))
	{
		if (shell_read_input(elem, shell) && shell_set_output(elem, shell))
		{
			read_lexing(*cmd);
			if (elem->args && (built_in = shell_builtin(elem, shell)) == -1)
				return (-1);
			if (!built_in && !elem->exec)
				ft_dprintf(2, "21sh: %s: command not found\n", elem->args[0]);
			else if (!built_in && elem->exec)
				shell_exec(tmp_fd, elem, shell);
		}
	}
	clean_cmd(cmd);
	ft_strdel(&shell->str);
	return (1);
}


void	read_lexing(t_cmd *cmd)
{
	t_output	*read;
	int 		i;

	while ((cmd = cmd->next_cmd))
	{
		ft_dprintf(2, "Read exec : %s\n", cmd->exec);
		i = 0;
		ft_dprintf(2, "Read array : ");
		while (cmd->args[i])
		{
			ft_dprintf(2, "arg[%i]=<%s> ", i, cmd->args[i]);
			i++;
		}
		ft_dprintf(2, "\nRead stdout : ");
		read = cmd->output;
		if (read == NULL)
			ft_dprintf(2, "(NULL)");
		while (read != NULL)
		{
			ft_dprintf(2, "from %d to <%s> append=%d - ", read->from, read->to, read->append);
			read = read->next;
		}

		ft_dprintf(2, "\nRead stdin : ");
		if (!cmd->input)
			ft_dprintf(2, "(NULL)");
		i = 0;
		while (cmd->input && (cmd->input)[i] != NULL)
		{
			if ((int)(cmd->input)[i] <= -1 && (int)(cmd->input)[i] >= -3)
				ft_dprintf(2, "<%d> -", (int)(cmd->input)[i++]);
			else
				ft_dprintf(2, "<%s> - ", (cmd->input)[i++]);
		}
		ft_dprintf(2, "\nRead heredoc : ");
		if (!cmd->hrdc)
			ft_dprintf(2, "(NULL)");
		i = 0;
		while (cmd->hrdc && (cmd->hrdc)[i] != NULL)
		{
			if ((int)(cmd->hrdc)[i] <= -1 && (int)(cmd->hrdc)[i] >= -3)
				ft_dprintf(2, "<%d> -", (int)(cmd->hrdc)[i++]);
			else
				ft_dprintf(2, "<%s> - ", (cmd->hrdc)[i++]);
		}
		if ((int)(cmd->process).fd_stdin == -1)
			ft_dprintf(2, "\nRead fd stdin : <%d>\n", (int)cmd->process.fd_stdin);
		else
			ft_dprintf(2, "\nRead fd stdin : <%s>\n", cmd->process.fd_stdin);
		ft_dprintf(2, "Et sep %d\n", cmd->sep);
		ft_dprintf(2, "-------------\n");
	}
}
