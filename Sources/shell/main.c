/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 19:23:25 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/22 13:26:42 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

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
		if (!stdout_to(next->output))
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

void	shell_save_histo(t_shell *shl)
{
	if (shl->hrdc_tmp)
		ft_strdel(&shl->str);
	shl->str = shl->hrdc_tmp ? ft_strdup(shl->hrdc_tmp) : shl->str;
	if (shl->str && ((!shl->hist->cmd && !shl->hist->prev) ||
					 (shl->hist->prev && shl->hist->prev->cmd &&
					  ft_strcmp(shl->hist->prev->cmd, shl->str))))
		shl->hist->cmd = ft_strdup(shl->str);
}

int		main(void)
{
	extern char **environ;
	t_shell		*shl;
	e_prompt	prmt;
	t_cmd		*cmd;
	e_shortcut	ret;

	if (!isatty(STDIN_FILENO))
		return (1);
	shell_init(&shl, &prmt, &cmd, environ);
	while (isatty(STDIN_FILENO) && (ret = get_stdin(shl, &prmt)) != -1)
	{
		if (!hrdc_fill(&prmt, &cmd, shl, ret) && !check_shrt(&prmt, ret, shl))
			break ;
		if ((shl->str && (cmd = shell_split(shl->str, shl->envp, &prmt))) ||
			(prmt == PROMPT && cmd && (cmd->process).stdin_send))
		{
			if (cmd_check(&cmd, shl, &prmt))
				continue;
			shell_save_histo(shl);
			if (check_syntax_err(cmd))
				shell_clean_data(&cmd, shl, 1, 1);
			else if (shell_process(&cmd, shl) == -1)
				break ;
		}
	}
	return (shell_exit(&cmd, &shl));
}
