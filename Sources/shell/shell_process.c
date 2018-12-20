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

#include "../../Include/shell.h"

#include <sys/types.h>
#include <signal.h>

void	shell_send_stdin(char **std_in, int tmp_fd[3])
{
	int i;

	close(tmp_fd[0]); // close the unused read side
	dup2(tmp_fd[1], 1); // connect the write side with stdout
	close(tmp_fd[1]); // close the write side
	i = 0;
	while (std_in && std_in[i])
		ft_printf("%s\n", std_in[i++]);
}

void	shell_prcs_get_stdin(int *tmp_fd)
{
	close(tmp_fd[1]); // close the unused write side
	dup2(tmp_fd[0], 0); // connect the read side with stdin
	close(tmp_fd[0]); // close the read side
}

void	shell_save_fd(int fd[3])
{
	fd[0] = dup(0);
	fd[1] = dup(1);
	fd[2] = dup(2);
}

void	reinit_fd(int fd[3])
{
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	dup2(fd[2], 2);
}

void	shell_prcs_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
}

void	shell_prcs_sigtstp(int signum)
{
	(void)signum;
	write(1, "\n[n]+  Stopped            process_name", 39);
	kill(0, SIGINT);
}

int		shell_process(t_cmd **cmd, t_shell *shell)
{
	t_cmd		*link;
	int			father;
	int 		fd[3];
	int			tmp_fd[3];

	if (!shell_prepare(*cmd, shell))
		return (shell_clean_data(cmd, shell, 1, 1));
	shell_save_fd(fd);
	read_lexing(*cmd);
	link = *cmd;
	while ((link = link->next_cmd))
	{
		pipe(tmp_fd);
		signal(SIGINT, shell_prcs_sigint);
		signal(SIGTSTP, shell_prcs_sigtstp);
		if (link->args && link->exec && (father = fork()) == 0)
		{
			if (link->std_in)
				shell_prcs_get_stdin(tmp_fd);
			//if (link->std_out)
			//	shell_send_stdout(tmp_fd, link->std_out);
			execve(link->exec, link->args, shell->envp);
		}
		else
		{
			if (link->std_in)
				shell_send_stdin(link->std_in, tmp_fd);
			reinit_fd(fd);
			wait(&father);
		}
		//shell_quotesub(*cmd);
		//shell_envpsub(&arg, envp, quote);
	}
	if (shell->str && ft_strcmp(shell->str, "exit") == 0)
	{
		clean_shell(&shell);
		clean_cmd(cmd);
		exit(1);
		//return((*cmd)->args[1]);
	}
	clean_cmd(cmd);
	ft_strdel(&shell->str);
	return (1);
}

void	read_lexing(t_cmd *cmd)
{
	t_stdout	*read;
	int 		i;

	while ((cmd = cmd->next_cmd))
	{
		dprintf(2, "Read exec : %s\n", cmd->exec);
		i = 0;
		dprintf(2, "Read array : ");
		while (cmd->args[i])
		{
			dprintf(2, "arg[%i]=<%s> ", i, cmd->args[i]);
			i++;
		}
		dprintf(2, "\nRead stdout : ");
		read = cmd->std_out;
		while (read != NULL)
		{
			dprintf(2, "from %d to <%s> append=%d - ", read->from, read->to, read->append);
			read = read->next;
		}
		dprintf(2, "\nRead stdin : ");
		i = 0;
		while (cmd->std_in && (cmd->std_in)[i] != NULL)
		{
			if ((int)(cmd->std_in)[i] <= -1 && (int)(cmd->std_in)[i] >= -3)
				dprintf(2, "<%d> -", (int)(cmd->std_in)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->std_in)[i++]);
		}
		dprintf(2, "\nRead heredoc : ");
		i = 0;
		while (cmd->hrdc && (cmd->hrdc)[i] != NULL)
		{
			if ((int)(cmd->hrdc)[i] <= -1 && (int)(cmd->hrdc)[i] >= -3)
				dprintf(2, "<%d> -", (int)(cmd->hrdc)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->hrdc)[i++]);
		}
		if ((int)cmd->hrdc_stdin == -1)
			dprintf(2, "\nRead heredoc stdin : <%d>\n", (int)cmd->hrdc_stdin);
		else
			dprintf(2, "\nRead heredoc stdin : <%s>\n", cmd->hrdc_stdin);
		dprintf(2, "Et sep %d\n", cmd->sep);
		dprintf(2, "-------------\n");
	}
}
