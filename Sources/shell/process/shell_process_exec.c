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

/*
** Le premier caractère de return_value correspond si exit est nécessaire (1) ou non (0)
** Les 10 char suivant sont la valeur de retour des builtin contenu dans un int
*/

void	shell_child(t_cmd *elem, t_shell *shell, int from_child[2])
{
	int 		tmp_fd[2];
	int 		is_builtin;
	char 		*return_value;
	char 		*tmp;

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
	else if (!is_builtin && elem->exec)
		execve(elem->exec, elem->args, shell->envp);
	tmp = ft_itoa(elem->ret);
	close(from_child[0]);
	if (is_builtin && ft_strcmp("exit", elem->args[0]) == 0)
		return_value = ft_strjoin("1", tmp);
	else
		return_value = ft_strjoin("0", tmp);
	ft_strdel(&tmp);
	write(from_child[1], return_value, (size_t)ft_strlen(return_value));
	exit(EXIT_SUCCESS);
}

/*
** Le premier caractère de buf correspond si exit est nécessaire (1) ou non (0)
** Les 10 char suivant sont la valeur de retour des builtin contenu dans un int
*/

BOOL	shell_father(t_cmd *elem, int pid_child, int from_child[2])
{
	char buf[12];

	wait(&pid_child);
	close(from_child[1]);
	ft_bzero(&buf, 11);
	read(from_child[0], buf, 12);
	elem->ret = ft_atoi(buf + 1);
	if (buf[0] == '1')
		return (0);
	else
		return (1);
}

int 	shell_exec(t_cmd *elem, t_shell *shell)
{
	int		father;
	int 	fd[3];
	int 	from_child[2];
	BOOL	exec_ok;

	exec_ok = 1;
	pipe(from_child);
	shell_save_fd(fd);
	if ((father = fork()) == 0)
		shell_child(elem, shell, from_child);
	else
		exec_ok = shell_father(elem, father, from_child);
	shell_reinit_fd(fd);
	if (!exec_ok)
		return (0);
	else
		return (1);
}

/*
int     main(int ac, char **av)
{
	int pfd2[2];
	int p;
	char *msg;
	char *buf;

	buf = malloc(sizeof(char) * 20);
	pipe(pfd2);
	if((p = fork()) == 0)//from child to parent
	{
		close(pfd2[0]);
		write(pfd2[1],"from child: how r u\0",20);
	}
	else
	{
		wait(&p);
		close(pfd2[1]);
		read(pfd2[0], buf, 20);
		printf("\nreceive |%s|",buf);
	}
}
 */