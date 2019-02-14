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

void	shell_child_return_value(int exec, t_cmd *elem, int is_builtin,
									int fd_child[2])
{
	char	*tmp;
	char	*return_value;

	tmp = ft_itoa(elem->val_ret);
	if (is_builtin == -1 && elem->sep != SPL_PIPE)
	{
		write(2, "exit\n", 5);
		return_value = ft_strjoin("2", tmp);
	}
	else if ((is_builtin && elem->val_ret > 0) ||
			(!is_builtin && WEXITSTATUS(exec) == 0))
		return_value = ft_strjoin("1", tmp);
	else
		return_value = ft_strjoin("0", tmp);
	ft_strdel(&tmp);
	close(fd_child[0]);
	write(fd_child[1], return_value, (size_t)ft_strlen(return_value) + 1);
	ft_strdel(&return_value);
}

/*
** Le premier caractère de return_value correspond si exit est nécessaire (0/1)
** Les 10 char suivant sont la valeur de retour des builtin contenu dans un int
*/

void	shell_child(t_cmd *elem, t_shell *shell, int ret_child[2],
						int fd_pipe[2])
{
	int send_stdin[2];
	int is_builtin;
	int exec;

	exec = 0;
	pipe(send_stdin);
	shell_plomberie(elem, send_stdin, fd_pipe);
	is_builtin = shell_builtin(elem, shell);
	if (!is_builtin && elem->exec && ft_strcmp("not found", elem->exec) == 0)
		ft_dprintf(2, "21sh: %s: command not found\n", elem->args[0]);
	else if (!is_builtin && elem->exec)
	{
		if ((exec = fork()) == 0)
			execve(elem->exec, elem->args, shell->envp);
		else
			wait(&exec);
	}
	shell_child_return_value(exec, elem, is_builtin, ret_child);
	exit(EXIT_SUCCESS);
}

/*
** Le fils a connecté le output de la command dans fd_pipe
** Lecture char by char pour capter l'EOF puis les assembler dans strjoin_buf
*/

char	*shell_father_read_out(int *fd_pipe)
{
	char	*stdin_nxt_elem;
	char	buf;
	char	strjoin_buf[BUFF_READ];
	int		i;

	dup2(fd_pipe[0], 0);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	stdin_nxt_elem = NULL;
	ft_bzero(strjoin_buf, BUFF_READ);
	i = 0;
	while (read(0, &buf, 1) == 1)
	{
		strjoin_buf[i++] = buf;
		if (i > BUFF_READ - 2)
		{
			ft_strjoin_free(&stdin_nxt_elem, strjoin_buf);
			ft_bzero(strjoin_buf, BUFF_READ);
			i = 0;
		}
	}
	ft_strjoin_free(&stdin_nxt_elem, strjoin_buf);
	return (stdin_nxt_elem);
}

/*
** buf[0] correspond à :
**		- command échec '0'
** 		- command réussi '1'
** 		- exit nécessaire '2'
** Les 3 char suivant sont la valeur de retour des builtins contenu dans un int
**
** shell_read_out lis les valeurs de retour de la commande et les stock dans
** l'input de la prochaine commande
*/

int		shell_father(t_cmd *elem, t_shell *shell, int pid_child,
						int ret_child[2], int fd_pipe[2])
{
	char	ret[5];

	wait(&pid_child);
	close(ret_child[1]);
	ft_bzero(ret, 5);
	read(ret_child[0], &ret, 5);
	if (elem->sep == SPL_PIPE)
		(elem->next_cmd)->process.stdin_send = shell_father_read_out(fd_pipe);
	elem->val_ret = ft_atoi(ret + 1);
	shell->ret = ft_atoi(ret + 1);
	return (ret[0] - 48);
}

/*
** On s'emmerde pas à exec les builtin lié à {env} et {cd} dans l'enfant car
** après ça va être la merde pour informer le père des modifications des {var}
** Qui plus est la lib pour partage mémoire entre père et fils est interdite
** father qui reprend une valeur permet de respecter la norme 4 vars
*/

int		shell_exec(t_cmd *elem, t_shell *shell)
{
	int	father;
	int fd[3];
	int ret_child[2];
	int fd_pipe[2];

	shell_save_fd(fd);
	pipe(ret_child);
	pipe(fd_pipe);
	if ((father = fork()) == 0)
		shell_child(elem, shell, ret_child, fd_pipe);
	else
		father = shell_father(elem, shell, father, ret_child, fd_pipe);
	shell_reinit_fd(fd);
	check_builtin_env_cd(elem, shell);
	return (father);
}
