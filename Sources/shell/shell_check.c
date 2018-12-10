/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_check.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/07 00:43:10 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/07 00:43:10 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

BOOL	shell_hrdc_quotesub(t_cmd **cmd, t_cmd *next, t_shell *shell,
							e_prompt *prompt)
{
	int i;

	*cmd = next;
	ft_strdel(&shell->str);
	*prompt = HRDC;
	i = 0;
	while (next->hrdc[i] != NULL)
		shl_quotesub(next->hrdc[i++]);
	return (1);
}

/*
** -1 pour un hrdc malloc mais interrompu par Ctrl-C/D
** -2 pour une valeur attendu manquante (cat <)
** On del shell->str car déjà exploité dans cmd->split
*/

BOOL	cmd_check(t_cmd **cmd, t_shell *shell, e_prompt *prompt)
{
	t_cmd	*next;

	*cmd = (*cmd)->start;
	next = *cmd;
	while ((next = next->next_cmd))
	{
		if (next->hrdc && ((int)next->hrdc[0] < -3 || (int)next->hrdc[0] > -1))
			return (shell_hrdc_quotesub(cmd, next, shell, prompt));
		if ((next->hrdc && (int)next->hrdc[0] == -1) ||
			(((next->std_in && (int)next->std_in[0] == -2) ||
			(next->hrdc && (int)next->hrdc[0] == -2)) &&
			next->next_cmd == NULL))
		{
			write(2, "21sh: syntax error near unexpected token `newline'\n",
					51);
			ft_strdel(&shell->str);
			clean_cmd(cmd);
			return (1);
		}
	}
	return (0);
}
