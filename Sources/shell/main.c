/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 19:23:25 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/30 15:35:58 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

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

	new = NULL;
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

BOOL 	prt_err_sep(int sep)
{
	write(2, "21sh: syntax error near unexpected token `", 42);
	if (sep == 1)
		write(2, "|'\n", 3);
	else if (sep == 2)
		write(2, ";'\n", 3);
	else if (sep == 3)
		write(2, "||'\n", 4);
	else if (sep == 4)
		write(2, "&&'\n", 4);
	return (1);
}

BOOL	check_syntax_err(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd;
	while ((next = next->next_cmd))
	{
		if (!ft_strlen(next->args[0]) && next->sep)
			return (prt_err_sep(next->sep));
		if (!stdout_to(next->std_out))
		{
			write(2, "21sh: syntax error near unexpected token `>'\n", 45);
			return (1);
		}
	}
	return (0);
}

int		main(void)
{
	extern char **environ;
	e_prompt	prompt;
	t_cmd		*cmd;
	t_shell		*shl;

	init_terminal_data();
	shl = init_shell(environ);
	prompt = PROMPT;
	while (get_stdin(&shl->str, &prompt, &shl->hist, shl->envp) != -2)
	{
		if (shl->str && (cmd = shell_split(shl->str, shl->envp, &prompt)))
		{
			if ((!shl->hist->cmd && !shl->hist->prev) ||
					(shl->hist->prev && shl->hist->prev->cmd &&
					ft_strcmp(shl->hist->prev->cmd, shl->str)))
				shl->hist->cmd = ft_strdup(shl->str);
			if (check_syntax_err(cmd))
				clean_cmd(&cmd); //et shl->str n'a as besoin d'etre free ?
			else
				shell_process(cmd, shl);
		}
	}
	if (shl->hist)
		fill_hist_file(shl->hist);
	return (1);
}
