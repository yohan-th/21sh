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

void 	shell_child_pipe(t_cmd *elem, t_shell *shell, int fd_pipe[2])
{
	int is_builtin;

	dup2(ft_atoi(elem->process.fd_stdin + 1), 0);
	//Verif si le prochain est toujours un pipe ?
	if (elem->sep == SPL_PIPE)
		dup2(fd_pipe[1], 1);
	if (shell_read_input(elem, shell) && shell_set_output(elem, shell))
	{
		//shell_plomberie(elem, fd_pipe[2]);
		close(fd_pipe[0]);
		is_builtin = shell_builtin(elem, shell);
		if (!is_builtin && ft_strcmp("not found", elem->exec) == 0)
		{
			ft_dprintf(2, "21sh: %s: command not found\n", elem->args[0]);
			exit(EXIT_FAILURE);
		}
		else if (!is_builtin)
			execve(elem->exec, elem->args, shell->envp);
		exit(EXIT_FAILURE);
	}
}

void 	shell_father_pipe(t_cmd *elem, t_shell *shell, int fd_pipe[2])
{
	close(fd_pipe[1]);
	if (ft_atoi(elem->process.fd_stdin + 1) != 0)
		close (ft_atoi(elem->process.fd_stdin + 1));
	if (elem->next_cmd)
		elem->next_cmd->process.fd_stdin = ft_strjoin("&", ft_itoa(fd_pipe[0]));
}

void	shell_exec_pipes(t_cmd **elem, t_shell *shell)
{
	int	fd_pipe[2];
	int	child;
	int	status;
	int stop ;

	stop = 0;
	while (1)
	{
		read_lexing(*elem);
		pipe(fd_pipe);
		if ((child = fork()) == 0)
			shell_child_pipe(*elem, shell, fd_pipe);
		else
			shell_father_pipe(*elem, shell, fd_pipe);
		*elem = (*elem)->next_cmd;
		if (stop == 1 || !(*elem))
			break ;
		if ((*elem)->sep != SPL_PIPE)
			stop = 1;
	}
	waitpid(child, &status, 0);
	while (wait(NULL) > 0)
		;
}

/*
** ret correspond au résultat de la commande
** 	- 0 fail
**  - 1 succeed
**  - 2 need exit
*/

int		shell_process(t_cmd **cmd, t_shell *shell)
{
	t_cmd	*elem;
	int 	fd[3];
	int 	is_builtin;

	shell_prepare(*cmd, shell);
	shell_save_fd(fd);
	signal(SIGINT, shell_prcs_sigint);
	elem = *cmd;
	while (elem && (elem = elem->next_cmd))
	{
		if (elem->sep == SPL_PIPE)
			shell_exec_pipes(&elem, shell);
		else if (shell_read_input(elem, shell) && shell_set_output(elem, shell))
		{
			is_builtin = shell_builtin(elem, shell);
			if (!is_builtin && ft_strcmp("not found", elem->exec) == 0)
				ft_dprintf(2, "21sh: %s: command not found\n", elem->args[0]);
			else if (!is_builtin)
				shell_exec(elem, shell);
			else if (is_builtin == -1)
				return (-1);
			if ((elem->ret == 1 && elem->sep == DBL_PIPE) ||
					(elem->ret == 0 && elem->sep == DBL_SPRLU))
				break ;
		}

	}
	shell_reinit_fd(fd);
	clean_cmd(cmd);
	ft_strdel(&shell->str);
	return (1);
}


void	read_lexing(t_cmd *elem)
{
	t_output	*read;
	int 		i;

	ft_dprintf(2, "-------------\n");
	ft_dprintf(2, "Read exec : %s\n", elem->exec);
	i = 0;
	ft_dprintf(2, "Read array : ");
	while (elem->args[i])
	{
		ft_dprintf(2, "arg[%i]=<%s> ", i, elem->args[i]);
		i++;
	}
	ft_dprintf(2, "\nRead output : ");
	read = elem->output;
	if (read == NULL)
		ft_dprintf(2, "(NULL)");
	while (read != NULL)
	{
		ft_dprintf(2, "from %d to <%s> append=%d - ", read->from, read->to, read->append);
		read = read->next;
	}

	ft_dprintf(2, "\nRead input : ");
	if (!elem->input)
		ft_dprintf(2, "(NULL)");
	i = 0;
	while (elem->input && (elem->input)[i] != NULL)
	{
		if ((int)(elem->input)[i] <= -1 && (int)(elem->input)[i] >= -3)
			ft_dprintf(2, "|%d| -", (int)(elem->input)[i++]);
		else
			ft_dprintf(2, "|%s| - ", (elem->input)[i++]);
	}
	ft_dprintf(2, "\nRead hrdc : ");
	if (!elem->hrdc)
		ft_dprintf(2, "(NULL)");
	i = 0;
	while (elem->hrdc && (elem->hrdc)[i] != NULL)
	{
		if ((int)(elem->hrdc)[i] <= -1 && (int)(elem->hrdc)[i] >= -3)
			ft_dprintf(2, "|%d| -", (int)(elem->hrdc)[i++]);
		else
			ft_dprintf(2, "|%s| - ", (elem->hrdc)[i++]);
	}
	if ((int)(elem->process).stdin_send == -1)
		ft_dprintf(2, "\nRead stdin : |%d|\n", (int)elem->process.stdin_send);
	else
		ft_dprintf(2, "\nRead stdin : |%s|\n", elem->process.stdin_send);
	ft_dprintf(2, "Read fd stdin : |%s|\n", elem->process.fd_stdin);
	ft_dprintf(2, "Read fd stdout : |%s|\n", elem->process.fd_stdout);
	ft_dprintf(2, "Read fileout : |%d|\n", elem->process.fd_fileout);
	ft_dprintf(2, "Read fd stderr : |%s|\n", elem->process.fd_stderr);
	ft_dprintf(2, "Read fileerr : |%d|\n", elem->process.fd_fileerr);
	ft_dprintf(2, "Et sep %d\n", elem->sep);
	ft_dprintf(2, "-------------\n");
}
