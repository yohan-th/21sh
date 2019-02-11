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

void	shell_plomberie(t_cmd *elem, int tmp_fd[2])
{
	pipe(tmp_fd);
	if ((elem->process).stdin_send)
		shell_pipe_stdin(tmp_fd, (elem->process).stdin_send);
	if ((elem->process).fd_stdout)
		shell_pipe_stdout(elem->process);
	if ((elem->process).fd_stderr)
		shell_pipe_stderr(elem->process);
}

void	shell_child_return_value(int exec, t_cmd *elem, int is_builtin, int fd[2])
{
	char	*tmp;
	char 	*return_value;

	tmp = ft_itoa(elem->val_ret);
	if (is_builtin == -1)
		return_value = ft_strjoin("2", tmp);
	else if ((is_builtin && elem->val_ret > 0) ||
			(!is_builtin && WEXITSTATUS(exec) == 0))
		return_value = ft_strjoin("1", tmp);
	else
		return_value = ft_strjoin("0", tmp);
	ft_strdel(&tmp);
	close(fd[0]);
	write(fd[1], return_value, (size_t)ft_strlen(return_value) + 1);
	ft_strdel(&return_value);
}

/*
** Le premier caractère de return_value correspond si exit est nécessaire (1) ou non (0)
** Les 10 char suivant sont la valeur de retour des builtin contenu dans un int
*/

void	shell_child(t_cmd *elem, t_shell *shell, int from_child[2])
{
	int 		tmp_fd[2];
	int 		is_builtin;
	int 		exec;

	shell_plomberie(elem, tmp_fd);
	is_builtin = shell_builtin(elem, shell);
	if (!is_builtin && elem->exec && ft_strcmp("not found", elem->exec) == 0)
		ft_dprintf(2, "21sh: %s: command not found\n", elem->args[0]);
	else if (!is_builtin && elem->exec && (exec = fork()) == 0)
		execve(elem->exec, elem->args, shell->envp);
	wait(&exec);
	shell_child_return_value(exec, elem, is_builtin, from_child);
	exit(EXIT_SUCCESS);
}

/*
** buf[0] correspond à :
**		- command échec '0'
** 		- command réussi '1'
** 		- exit nécessaire '2'
** Les 10 char suivant sont la valeur de retour des builtin contenu dans un int
*/

int		shell_father(t_cmd *elem, int pid_child, int from_child[2])
{
	char	buf;
	char	ret[5];
	int 	i;

	wait(&pid_child);
	ft_bzero(ret, 5);
	close(from_child[1]);
	buf = -1;
	i = 0;
	while (buf != '\0')
	{
		read(from_child[0], &buf, 1);
		ret[i++] = buf;

	}
	printf("-<ret|%s|>\n", ret);
	elem->val_ret = ft_atoi(ret + 1);
	return (ret[0] - 48);
}

int 	shell_exec(t_cmd *elem, t_shell *shell)
{
	int		father;
	int 	fd[3];
	int 	from_child[2];
	int 	ret;

	ret = 0;
	pipe(from_child);
	shell_save_fd(fd);
	if ((father = fork()) == 0)
		shell_child(elem, shell, from_child);
	else
		ret = shell_father(elem, father, from_child);
	shell_reinit_fd(fd);
	return (ret);
}
