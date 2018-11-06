/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 19:23:25 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/06 18:14:37 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void	read_array(char **str)
{
	int i;

	i = 0;
	dprintf(2, "Read array : ");
	while (str[i])
	{
		dprintf(2, "arg[%i]=<%s> ", i, str[i]);
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

void	fill_hist(t_history **hist, char *line)
{
	t_history *new;
	t_history *now;

	(*hist)->next = new;
	now = *hist;
	*hist = new;
	(*hist)->cmd = ft_strdup(line);
	(*hist)->prev = now;
	(*hist)->next = NULL;
}

int		main(int ac, char **av, char **envp)
{
	e_prompt	prompt;
	t_cmd		*cmd;
	t_shell		*shell;

	init_terminal_data();
	shell = init_shell(envp);
	prompt = PROMPT;
	while (get_stdin(&shell->str, &prompt, &shell->hist) != -2)
	{
		if (!(cmd = shell_split(shell->str, shell->envp)))
			prompt = B_QUOTE;
		else if (shell->str)
		{
			if ((!shell->hist->cmd && !shell->hist->prev) ||
			(shell->hist->prev && shell->hist->prev->cmd &&
			ft_strcmp(shell->hist->prev->cmd, shell->str)))
				shell->hist->cmd = ft_strdup(shell->str);
			prompt = PROMPT;
			shell_process(cmd, shell);
		}
	}
	return (1);
}
