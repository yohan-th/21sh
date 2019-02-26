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

/*
** return :
**   0 --> ok elem suivant
**   1 --> elem fail ou un séparateur stop l'execution des elem
**  -1 --> un exit est nécessaire
*/

int		shell_exec(t_cmd *elem, t_shell *shell)
{
	int	is_builtin;

	if (!shell_read_input(elem, shell) || !shell_set_output(elem, shell))
		return (1);
	shell_plomberie(elem->process);
	is_builtin = shell_builtin(elem, shell);
	if (!is_builtin && elem->exec &&
		ft_strcmp("not found", elem->exec) == 0)
	{
		ft_dprintf(2, "21sh: %s: command not found\n", elem->args[0]);
		elem->ret = 1;
	}
	else if (!is_builtin && elem->exec &&
				ft_strcmp("directory", elem->exec) == 0)
	{
		ft_dprintf(2, "21sh: %s: Is a directory\n", elem->args[0]);
		elem->ret = 1;
	}
	else if (!is_builtin && elem->exec)
		shell_execve(elem, shell);
	else if (is_builtin == -1)
		return (-1);
	return (elem->ret);
}

/*
** elem->ret se prend -2 si builtin exit fail, on le corrige pour 1
** puis on rajoute elem->ret dans shell->envl
*/

void	shell_ret(t_cmd *elem, t_shell *shell)
{
	char *tmp;

	elem->ret = (elem->ret == -2) ? 1 : elem->ret;
	tmp = ft_itoa(elem->ret);
	if (!(check_replace_env_variable(&shell->envl, "?", tmp)))
		shell->envl = append_key_env(shell->envl, "?", tmp);
	ft_strdel(&tmp);
}

int		shell_process(t_cmd **cmd, t_shell *shell)
{
	t_cmd	*elem;
	int		fd[3];
	int		exec;

	shell_prepare(*cmd, shell);
	shell_save_fd(fd);
	signal(SIGINT, shell_prcs_sigint);
	elem = *cmd;
	while (elem && (elem = elem->next_cmd))
	{
		if (elem->sep == SPL_PIPE)
			exec = shell_exec_pipes(&elem, shell);
		else
			exec = shell_exec(elem, shell);
		shell_ret(elem, shell);
		if (exec == -1)
			return (-1);
		else if ((exec == EXIT_SUCCESS && elem->sep == DBL_PIPE) ||
			(exec > 0 && elem->sep == DBL_SPRLU))
			elem = elem->next_cmd;
	}
	shell_reinit_fd(fd);
	shell_clean_data(cmd, shell, 1);
	return (1);
}
