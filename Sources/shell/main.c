/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 19:23:25 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/03 14:57:27 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int		init_terminal_data(void)
{
	static char	term_buffer[2048];
	char		*termtype;
	int			success;

	termtype = getenv("TERM");
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

BOOL	check_syntax_err(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd;
	while (next && (next = next->next_cmd))
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
			return (1);
		}
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
	int 		ret;

	init_terminal_data();
	shl = init_shell(environ);
	prompt = PROMPT;
	cmd = NULL;
	while ((ret = get_stdin(&shl->str, &prompt, &shl->hist, shl->envp)) != -1)
	{
		hrdc_fill(&prompt, cmd, shl, ret);
		if (ret == -2)
			break ;
		else if (ret == -3)
			ft_strdel(&shl->str);
		if (shl->str && (cmd = shell_split(shl->str, shl->envp, &prompt)))
		{
		//	read_lexing(cmd);
			if (hrdc_check(&cmd, shl, &prompt))
				continue ;
			if ((!shl->hist->cmd && !shl->hist->prev) ||
						(shl->hist->prev && shl->hist->prev->cmd &&
						ft_strcmp(shl->hist->prev->cmd, shl->str)))
				shl->hist->cmd = ft_strdup(shl->str);
			if (check_syntax_err(cmd))
				clean_data(cmd, shl, 1, 1);
			else
				shell_process(cmd, shl);
		}
	}
	//ft_exit
	if (shl->hist)
		fill_hist_file(shl->hist);
	return (1);
}

void	read_lexing(t_cmd *cmd)
{
	t_stdout	*read;
	int 		i;
	while ((cmd = cmd->next_cmd))
	{
		i = 0;
		dprintf(2, "Read array : ");
		while (cmd->args[i])
		{
			dprintf(2, "arg[%i]=<%s> ", i, cmd->args[i]);
			i++;
		}
		dprintf(2, "\nRead stdout : ");
		read = cmd->std_out;
		while (read != NULL)
		{
			dprintf(2, "from %d to <%s> append=%d - ", read->from, read->to, read->append);
			read = read->next;
		}
		dprintf(2, "\nRead stdin : ");
		i = 0;
		while (cmd->std_in && (cmd->std_in)[i] != NULL)
		{
			if ((int)(cmd->std_in)[i] == -1 || (int)(cmd->std_in)[i] == -2)
				dprintf(2, "<%d> -", (int)(cmd->std_in)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->std_in)[i++]);
		}
		dprintf(2, "\nRead heredoc : ");
		i = 0;
		while (cmd->hrdc && (cmd->hrdc)[i] != NULL)
		{
			if ((int)(cmd->hrdc)[i] == -1 || (int)(cmd->hrdc)[i] == -2)
				dprintf(2, "<%d> -", (int)(cmd->hrdc)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->hrdc)[i++]);
		}
		dprintf(2, "\nRead heredoc stdin : <%s>\n", cmd->hrdc_stdin);
		dprintf(2, "Et sep %d\n", cmd->sep);
		dprintf(2, "-------------\n");
	}
}
