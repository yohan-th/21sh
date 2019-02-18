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

/*
** Le premier caractère de return_value correspond si exit est nécessaire (0/1)
** Les 10 char suivant sont la valeur de retour des builtin contenu dans un int
*/

void	shell_child(t_cmd *elem, t_shell *shell)
{
	int send_stdin[2];

	pipe(send_stdin);
	shell_plomberie(elem, send_stdin);
	execve(elem->exec, elem->args, shell->envp);
}

/*
** buf[0] correspond à :
**		- command échec '0'
** 		- command réussi '1'
** 		- exit nécessaire '2'
** Les 3 char suivant sont la valeur de retour des builtins contenu dans un int
*/

int		shell_father(t_cmd *elem, t_shell *shell, int pid_child)
{
	wait(&pid_child);
	return (WEXITSTATUS(pid_child));
}
/*
** On retourne 0 si execve fail ou 1 si succeed
*/

void		shell_exec(t_cmd *elem, t_shell *shell)
{
	int	child;
	int fd[3];
	int ret_child[2];

	shell_save_fd(fd);
	pipe(ret_child);
	if ((child = fork()) == 0)
		shell_child(elem, shell);
	else
		elem->ret = shell_father(elem, shell, child) ? 0 : 1;
	shell_reinit_fd(fd);
}
