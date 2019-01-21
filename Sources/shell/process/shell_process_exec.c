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

int 	shell_exec(int tmp_fd[3], t_cmd *elem, t_shell *shell)
{
	int		father;
	int 	fd[3];

	pipe(tmp_fd);
	if ((father = fork()) == 0)
		shell_child(elem, shell, tmp_fd);
	else
		shell_father(elem, tmp_fd, father, fd);
	return (1);
}
