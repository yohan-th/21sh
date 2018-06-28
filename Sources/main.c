/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 19:23:25 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/04 19:23:25 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"

int		shell_error(char *type)
{
	if (ft_strcmp(type, "setenv usage") == 0)
		ft_printf("minishell: setenv: invalid argument\n"
					"usage: setenv VAR VALUE\n");
	else if (ft_strcmp(type, "unsetenv usage") == 0)
		ft_printf("minishell: unsetenv: invalid argument\n"
					"usage: unsetenv VAR\n");
	else if (ft_strcmp(type, "env usage") == 0)
		ft_printf("minishell: env: invalid argument\nusage: env VAR\n");
	else if (ft_strcmp(type, "$HOME env not set") == 0)
		ft_printf("minishell: cd ~: $HOME env not set\n");
	return (0);
}

/*
** Lorsqu'on lance shell, OLDPWD ne doit pas exister, on le del de dup_envp
*/

char	*readline(t_shell *shell)
{
	char	*line;
	int 	gnl;

	line = NULL;
	gnl = get_next_line(0, &line);
	if (gnl == -1 || line == NULL)
	{
		ft_arrdel(shell->envp);
		ft_strdel(&shell->str);
		write(1, "\n", 1);
		exit(1);
	}
	if (shell->str)
		shell->str = ft_strjoin_mltp(3, shell->str, "\n", line);
	else
		shell->str = ft_strdup(line);
	ft_strdel(&line);
	return (shell->str);
}

int		prompt(int multi)
{
	if (multi == 0)
		ft_printf("$>");
	else
		ft_printf(">");
	return (1);
}

int		main(int ac, char **av, char **envp)
{
	t_cmd	*cmd;
	t_shell	*shell;

	shell = init_shell(envp);
	while (prompt(shell->mltline) && (shell->str = readline(shell)))
	{
		if (!(cmd = shell_split(shell->str, shell->envp)))
			shell->mltline = 1;
		else
			shell_process(cmd, shell);
	}
	return (1);
}
