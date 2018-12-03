/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_hrdc_check.c                               .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/02 17:55:46 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/02 17:55:46 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

char 	*get_next_hrdc(char **hrdc)
{
	int		i;

	i = 0;
	while (hrdc && (int)hrdc[i] == -1)
		i++;
	return (hrdc ? hrdc[i] : NULL);
}

int 	del_next_hrdc(char **hrdc)
{
	int		i;

	i = 0;
	while (hrdc && (int)hrdc[i] == -1)
		i++;
	if (hrdc && hrdc[i])
	{
		ft_strdel(&hrdc[i]);
		hrdc[i] = (char *)-1;
	}
	return (1);
}

int		hrdc_fill(e_prompt *prompt, t_cmd *cmd, t_shell *shell, int shortcut)
{
	char	*hrdc;

	if (shortcut == 3 && *prompt == PROMPT && cmd)  //HRDC + Ctrl+C
		return (clean_data(cmd, shell, 1, 1));
	if (*prompt == HRDC && cmd)
	{
		hrdc = get_next_hrdc(cmd->hrdc);
		if (hrdc && ft_strcmp(hrdc, shell->str) == 0)
		{
			del_next_hrdc(cmd->hrdc);
			if (get_next_hrdc(cmd->hrdc) == NULL)
				*prompt = PROMPT;
			read_lexing(cmd->start);
		}
		else
		{
			ft_strjoin_free(&cmd->hrdc_stdin, shell->str);
			//ft_strjoin_free(&cmd->hrdc_stdin, "\n");
		}
		ft_strdel(&shell->str);
	}
	return (1);
}

BOOL	hrdc_check(t_cmd **cmd, t_shell *shell, e_prompt *prompt)
{
	t_cmd	*next;

	next = *cmd;
	while ((next = next->next_cmd))
	{
		if (next->hrdc)
		{
			*cmd = next;
			ft_strdel(&shell->str);
			*prompt = HRDC;
			return (1);
		}
	}
	return (0);
}
