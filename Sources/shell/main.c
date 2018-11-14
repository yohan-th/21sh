/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 19:23:25 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/07 14:09:51 by dewalter    ###    #+. /#+    ###.fr     */
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

void	fill_hist_file(t_history *hist)
{
	int fd;

	if ((fd = open(".21sh_history", O_WRONLY | O_CREAT |
	O_APPEND | O_TRUNC, 0644)) < 0)
		return ;
	while (hist)
	{
		if (hist->cmd)
		{
			write(fd, hist->cmd, ft_strlen(hist->cmd));
			write(fd, "\n", 1);
		}
		hist = hist->prev;
	}
	close(fd);
}

int 	prt_err_sep(int sep)
{

	return (1);
}

BOOL	check_syntax_err(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd;
	while ((next = next->next_cmd))
	{
		if (!ft_strlen(next->args[0]) && next->sep)
		{
			write(2, "21sh: syntax error near unexpected token `", 42);
			if (next->sep == 1)
				write(2, "|'\n", 3);
			else if (next->sep == 2)
				write(2, ";'\n", 3);
			else if (next->sep == 3)
				write(2, "||'\n", 4);
			else if (next->sep == 4)
				write(2, "&&'\n", 4);
			return (0);
		}
		if (!checkredi_to(next->redi))
		{
			write(2, "21sh: syntax error near unexpected token `>'\n", 45);
			return (0);
		}
	}
	return (1);
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
		if ((cmd = shell_split(shell->str, shell->envp, &prompt)))
		{
			if ((!shell->hist->cmd && !shell->hist->prev) ||
					(shell->hist->prev && shell->hist->prev->cmd &&
					ft_strcmp(shell->hist->prev->cmd, shell->str)))
				shell->hist->cmd = ft_strdup(shell->str);
			if (check_syntax_err(cmd))
				shell_process(cmd, shell);
		}
	}
	if (shell->hist)
		fill_hist_file(shell->hist);
	return (1);
}
