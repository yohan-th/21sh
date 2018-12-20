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

BOOL	check_shrt(e_prompt *prompt, e_shortcut shortcut, t_shell *shl)
{
	char quote;

	if (shortcut == CTRLC && prompt != PROMPT)
		*prompt = PROMPT;
	if (shortcut == CTRLD || shortcut == CTRLC)
		ft_strdel(&shl->str);
	if (shortcut == CTRLD && *prompt == PROMPT)
		return (0);
	else if (shortcut == CTRLD && *prompt != PROMPT)
	{
		quote = (*prompt == S_QUOTE) ? (char)'\'' : (char)'"';
		write(2, "bash: unexpected EOF while looking for matching `", 50);
		write(2, &quote, 1);
		write(2, "'\n", 2);
		write(2, "bash: syntax error: unexpected end of file\n", 43);
		*prompt = PROMPT;
	}
	return (1);
}

void	shell_init(t_shell **shell, e_prompt *prompt, t_cmd **cmd, char ** env)
{
	init_terminal_data();
	*shell = init_shell(env);
	*prompt = PROMPT;
	*cmd = NULL;
}

int	shell_exit(t_cmd **cmd, t_shell **shell)
{
	clean_cmd(cmd);
	if ((*shell)->hist)
		fill_hist_file((*shell)->hist);
	clean_shell(shell);
	return (5);
}

int		main(void)
{
	extern char **environ;
	t_shell		*shl;
	e_prompt	prmpt;
	t_cmd		*cmd;
	e_shortcut	ret;

	shell_init(&shl, &prmpt, &cmd, environ);
	while ((ret = get_stdin(&shl->str, &prmpt, &shl->hist, shl->envp)) != -1)
	{
		if (!hrdc_fill(&prmpt, &cmd, shl, ret) && !check_shrt(&prmpt, ret, shl))
			break ;
		if ((shl->str && (cmd = shell_split(shl->str, shl->envp, &prmpt))) ||
				(prmpt == PROMPT && cmd && cmd->hrdc_stdin))
		{
			if (cmd_check(&cmd, shl, &prmpt))
				continue;
			if (shl->str && ((!shl->hist->cmd && !shl->hist->prev) ||
						(shl->hist->prev && shl->hist->prev->cmd &&
						ft_strcmp(shl->hist->prev->cmd, shl->str))))
				shl->hist->cmd = ft_strdup(shl->str);
			if (check_syntax_err(cmd))
				shell_clean_data(&cmd, shl, 1, 1);
			else if (shell_process(&cmd, shl) == -1)
				break ;
		}
	}
	return (shell_exit(&cmd, &shl));
}
