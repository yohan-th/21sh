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

void	hrdc_fill_stdin(e_prompt *prompt, t_cmd *cmd, t_shell *shell)
{
	char	*hrdc;

	if (cmd->hrdc_stdin == NULL)
		cmd->hrdc_stdin = ft_strdup("");
	hrdc = get_next_hrdc(cmd->hrdc);
	if (hrdc && shell->str && ft_strcmp(hrdc, shell->str) == 0)
	{
		del_next_hrdc(cmd->hrdc);
		if (get_next_hrdc(cmd->hrdc) == NULL)
			*prompt = PROMPT;
	}
	else
	{
		ft_strjoin_free(&cmd->hrdc_stdin, shell->str);
		//ft_strjoin_free(&cmd->hrdc_stdin, "\n");
	}
	ft_strdel(&shell->str);
}

/*
** La première conditon correspondau Ctrl-C dans un heredoc
** La deuxième à un Ctrl-D (si cmd existe alors il attend des heredoc)
*/

int		hrdc_fill(e_prompt *prompt, t_cmd *cmd, t_shell *shell, e_shortcut ret)
{
	if (ret == CTRLC && *prompt == PROMPT && cmd)  //HRDC + Ctrl+C
		return (clean_data(cmd, shell, 1, 1));
	if (ret == CTRLD && *prompt == HRDC && !shell->str)
	{
		while (get_next_hrdc(cmd->hrdc))
			del_next_hrdc(cmd->hrdc);
		cmd->hrdc = NULL;
		*prompt = PROMPT;
		return (1);
	}
	if (*prompt == HRDC && cmd)
	{
		hrdc_fill_stdin(prompt, cmd, shell);
		return (1);
	}
	return (0);
}

/*
** On del shell->str car déjà exploité dans cmd->split
*/

BOOL	hrdc_check(t_cmd **cmd, t_shell *shell, e_prompt *prompt)
{
	t_cmd	*next;

	*cmd = (*cmd)->start;
	next = *cmd;
	while ((next = next->next_cmd))
	{
		if (next->hrdc && (int)next->hrdc[0] != -1)
		{
			*cmd = next;
			ft_strdel(&shell->str);
			*prompt = HRDC;
			return (1);
		}
	}
	return (0);
}
