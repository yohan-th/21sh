/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process_tools.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/11 01:01:54 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 01:01:54 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

void	shell_prcs_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
}

void	check_builtin_env_cd(t_cmd *elem, t_shell *shell)
{
	if (elem->args[0] && ft_strcmp("cd", elem->args[0]) == 0)
		builtin_cd(elem->args, &shell->envp, 0);
	else if (elem->args[0] && ft_strcmp("setenv", elem->args[0]) == 0 &&
				elem->args[1] != NULL && !elem->args[2])
		builtin_setenv(&shell->envp, elem->args[0], elem->args[1]);
	else if (elem->args[0] && ft_strcmp("unsetenv", elem->args[0]) == 0 &&
				elem->args[0] != NULL && elem->args[1] == NULL &&
				get_envp(shell->envp, elem->args[0]))
		shell->envp = rmv_key_env(shell->envp, elem->args[0]);
}
