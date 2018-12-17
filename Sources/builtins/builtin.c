/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/23 13:13:48 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/23 13:13:48 by ythollet    ###    #+. /#+    ###.fr     */
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
		builtin_env(envp, NULL);
	else if (cmd[1] != NULL && cmd[2] == NULL)
		builtin_env(envp, cmd[1]);
	else
		shell_error("env usage", 0);
}

/*
** Si on on rentre dans un builtin on retourne NULL
** pour ne pas lancer de fork
*/

char	*check_builtin(char **cmd, char ***envp)
{
	if (*cmd && ft_strcmp("echo", cmd[0]) == 0)
		builtin_echo(cmd);
	else if (*cmd && ft_strcmp("cd", cmd[0]) == 0)
		builtin_cd(cmd, envp);
	else if (*cmd && ft_strcmp("setenv", cmd[0]) == 0)
		check_builtin_setenv(envp, cmd);
	else if (*cmd && ft_strcmp("unsetenv", cmd[0]) == 0)
		check_builtin_unsetenv(envp, cmd);
	else if (*cmd && ft_strcmp("env", cmd[0]) == 0)
		check_builtin_env(envp, cmd);
	else if (*cmd && ft_strcmp("exit", cmd[0]) == 0)
		return ("exit");
	else
		return (NULL);
	return ("build done");
}
