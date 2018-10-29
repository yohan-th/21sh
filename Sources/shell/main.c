/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 19:23:25 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/29 23:48:48 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"

/*
** Lorsqu'on lance shell, OLDPWD ne doit pas exister, on le del de dup_envp
*/

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

int		init_terminal_data(void)
{
	static char term_buffer[2048];
	char *termtype = getenv ("TERM");
	int success;

	if (termtype == 0)
	{
		ft_putstr("Specify a terminal type with `setenv TERM <yourtype>'.\n");
		return (EXIT_FAILURE);
	}
	success = tgetent(term_buffer, termtype);
	if (success < 0)
	{
		ft_putstr("Could not access the termcap data base.\n");
		return (EXIT_FAILURE);
	}
	if (success == 0)
	{
		ft_putstr("Terminal type `");
		ft_putstr(termtype);
		ft_putendl("' is not defined.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		main(int ac, char **av, char **envp)
{
	e_prompt prompt;
	t_cmd	*cmd;
	t_shell	*shell;

	init_terminal_data();
	shell = init_shell(envp);
	prompt = PROMPT;
	shell->str = NULL;
	while (get_stdin(&shell->str, &prompt) != -2)
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
