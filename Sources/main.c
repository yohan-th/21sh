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

/*
** Lorsqu'on lance shell, OLDPWD ne doit pas exister, on le del de dup_envp
*/

char	*readline(t_shell *shell)
{
	char	*line;
	int		gnl;
	char	*tmp;

	line = NULL;
	gnl = get_next_line(0, &line);
	if (gnl == -1 || line == NULL)
		shell_error("gnl", 2, &shell->str, shell->envp);
	if (shell->str)
	{
		tmp = shell->str;
		shell->str = ft_strjoin_mltp(3, shell->str, "\n", line);
		ft_strdel(&tmp);
	}
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

void	read_array(char **str)
{
	int i;

	i = 0;
	ft_printf("Read array : ");
	while (str[i])
	{
		ft_printf("arg[%i]=<%s> ", i, str[i]);
		i++;
	}
}

int		main(int ac, char **av, char **envp)
{
	t_cmd	*cmd;
	t_shell	*shell;

	shell = init_shell(envp);
	while (prompt(shell->mltline) && (shell->str = readline(shell)))
	{
		if (!(cmd = shell_split(shell->str, shell->envp)))
		{
			printf("multiline\n");
			shell->mltline = 1;
		}
		else
		{
			shell->mltline = 0;
			shell_process(cmd, shell);
		}
	}
	return (1);
}
