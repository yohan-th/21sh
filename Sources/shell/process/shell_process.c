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

#include <sys/types.h>
#include <signal.h>

void	shell_prcs_get_stdin(int *tmp_fd)
{
	close(tmp_fd[1]); // close the unused write side
	dup2(tmp_fd[0], 0); // connect the read side with stdin
	close(tmp_fd[0]); // close the read side
}

/*
void	shell_send_stdout(int *tmp_fd, t_stdout *std_out)
{
	;
}
*/

void	shell_child(t_cmd *elem, t_shell *shell, int tmp_fd[3])
{
	if ((elem->process).fd_stdin)
		shell_prcs_get_stdin(tmp_fd);
//	if (elem->std_out)
//		shell_send_stdout(tmp_fd, elem->std_out);
	execve(elem->exec, elem->args, shell->envp);
}

void	shell_send_stdin(t_cmd *elem, int tmp_fd[3])
{
	close(tmp_fd[0]); // close the unused read side
	dup2(tmp_fd[1], 1); // connect the write side with stdout
	close(tmp_fd[1]); // close the write side
	ft_putstr((elem->process).fd_stdin);
}


void	shell_father(t_cmd *elem, int tmp_fd[3], int pid_child, int fd[3])
{
	if (elem->input)
		shell_send_stdin(elem, tmp_fd);
	reinit_fd(fd);
	wait(&pid_child);
}

int		shell_process(t_cmd **cmd, t_shell *shell)
{
	t_cmd	*elem;
	int		father;
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
			pipe(tmp_fd);
			if (!built_in && elem->args && elem->exec && (father = fork()) == 0)
				shell_child(elem, shell, tmp_fd);
			else if (!built_in && elem->args && elem->exec)
				shell_father(elem, tmp_fd, father, fd);
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
