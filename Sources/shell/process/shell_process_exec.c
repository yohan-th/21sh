/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process_exec.c                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/01/21 22:44:23 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/21 22:44:23 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

void	shell_pipe_stdin(int tmp_fd[2], char *send_stdin)
{
	ft_putstr_fd(send_stdin, tmp_fd[1]);
	close(tmp_fd[1]);
	dup2(tmp_fd[0], 0);
}

void	shell_pipe_stdout(t_process process)
{
	if (process.fd_stdout[0] == '&' && process.fd_stdout[1] != '1')
		dup2(ft_atoi(process.fd_stdout + 1), 1); // connect the write side with file
	else if (process.fd_fileout != 0)
		dup2(process.fd_fileout, 1);
}

void	shell_pipe_stderr(t_process process)
{
	if (process.fd_stderr[0] == '&' && process.fd_stderr[1] != '2')
		dup2(ft_atoi(process.fd_stderr + 1), 2); // connect the write side with file
	else if (process.fd_fileerr != 0)
		dup2(process.fd_fileerr, 2);
}


void	shell_child(t_cmd *elem, t_shell *shell, int ret_fd[2])
{
	int 		tmp_fd[2];
	int 		is_builtin;

	pipe(tmp_fd);
	if ((elem->process).stdin_send)
		shell_pipe_stdin(tmp_fd, (elem->process).stdin_send);
	if ((elem->process).fd_stdout)
		shell_pipe_stdout(elem->process);
	if ((elem->process).fd_stderr)
		shell_pipe_stderr(elem->process);
	is_builtin = shell_builtin(elem, shell);
	if (!is_builtin && elem->exec && ft_strcmp("not found", elem->exec) == 0)
		ft_dprintf(2, "21sh: %s: command not found\n", elem->args[0]);
	else if (!is_builtin)
		execve(elem->exec, elem->args, shell->envp);

	/*
	printf("-<|child send|>\n");
	close(ret_fd[0]); // close the read-end of the pipe, I'm not going to use it
	write(ret_fd[1], "send by child", 13); // send the content of argv[1] to the reader
	write(ret_fd[1], "\n", 1);
	write(1, "\n", 1);
	close(ret_fd[1]); // close the write-end of the pipe, thus sending EOF to the reader
	printf("-<|END|>\n");
	*/
	exit(EXIT_SUCCESS);
}

void	shell_father(t_cmd *elem, int pid_child, int ret_fd[2])
{
	char buf;
	/*
	close(ret_fd[1]); // close the write-end of the pipe, I'm not going to use it
	while (read(ret_fd[0], &buf, 1) > 0) // read while EOF
		write(1, &buf, 1);
	write(1, "\n", 1);
	close(ret_fd[0]); // close the read-end of the pipe
	 */
	wait(&pid_child);
}

int 	shell_exec(t_cmd *elem, t_shell *shell)
{
	int		father;
	int 	fd[3];
	int 	ret_fd[2];

	pipe(ret_fd);
	shell_save_fd(fd);
	if ((father = fork()) == 0)
		shell_child(elem, shell, ret_fd);
	else
		shell_father(elem, father, fd);
	shell_reinit_fd(fd);
	if (father == 0 && elem->ret == -1)
		return (-1);
	else
		return (1);
}
