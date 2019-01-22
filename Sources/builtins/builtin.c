/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/23 13:13:48 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/17 13:45:22 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void	check_builtin_setenv(char ***envp, char **cmd)
{
	if (cmd[1] != NULL && cmd[2] != NULL && cmd[3] == NULL)
		builtin_setenv(envp, cmd[1], cmd[2]);
	else
		shell_error("env set usage", 0);
}

void	check_builtin_unsetenv(char ***envp, char **cmd)
{
	if (cmd[1] != NULL && cmd[2] == NULL && get_envp(*envp, cmd[1]))
		*envp = rmv_key_env(*envp, cmd[1]);
	else
		shell_error("env unset usage", 0);
}

void	check_builtin_env(char ***envp, char **cmd)
{
	if (cmd[1] == NULL)
		builtin_env(*envp, NULL);
	else if (cmd[1] != NULL && cmd[2] == NULL)
		builtin_env(*envp, cmd[1]);
	else
		shell_error("env usage", 0);
}

/*
** Si on on rentre dans un builtin on retourne NULL
** pour ne pas lancer de fork
*/

int 	check_shell_variable(char *arg)
{
	int i;

	i = -1;
	while (arg[++i])
		if (arg[i] == '=')
			return (1);
	return (0);
}

int		shell_builtin(t_cmd *link, t_shell *shell)
{
	if (link->args[0] && ft_strcmp("echo", link->args[0]) == 0)
		builtin_echo(link->args);
	else if (link->args[0] && ft_strcmp("cd", link->args[0]) == 0)
		builtin_cd(link->args, &shell->envp);
	else if (link->args[0] && ft_strcmp("setenv", link->args[0]) == 0)
		check_builtin_setenv(&shell->envp, link->args);
	else if (link->args[0] && ft_strcmp("unsetenv", link->args[0]) == 0)
		check_builtin_unsetenv(&shell->envp, link->args);
	else if (link->args[0] && ft_strcmp("env", link->args[0]) == 0)
		check_builtin_env(&shell->envp, link->args);
	else if (link->args[0] && check_shell_variable(link->args[0]))
		builtin_env_all(&shell->envp, &shell->envl, link->args);
	else if (link->args[0] && ft_strcmp("exit", link->args[0]) == 0)
		return (-1);
	else
		return (0);
	return (1);
}
