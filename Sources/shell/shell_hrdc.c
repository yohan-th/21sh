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

char	*get_next_hrdc(char **hrdc)
{
	int	i;

	i = 0;
	while (hrdc && (int)hrdc[i] == -3)
		i++;
	return (hrdc ? hrdc[i] : NULL);
}

/*
** hrdc[i] prend la valeur de -3 pour indiqué d'être rempli
*/

int		del_next_hrdc(char **hrdc)
{
	int	i;

	i = 0;
	while (hrdc && (int)hrdc[i] == -3)
		i++;
	if (hrdc && hrdc[i])
	{
		ft_strdel(&hrdc[i]);
		hrdc[i] = (char *)-3;
	}
	return (1);
}

void	hrdc_fill_stdin(e_prompt *prompt, t_cmd **cmd, t_shell *shell)
{
	char	*hrdc;
	int		last;

	if ((int)((*cmd)->process).stdin_send == -1)
		((*cmd)->process).stdin_send = ft_strdup("");
	hrdc = get_next_hrdc((*cmd)->hrdc);
	if (hrdc && shell->str && ft_strcmp(hrdc, shell->str) == 0)
	{
		del_next_hrdc((*cmd)->hrdc);
		if (get_next_hrdc((*cmd)->hrdc) == NULL)
		{
			*prompt = PROMPT;
			last = 0;
			while ((int)(*cmd)->input[last] != -1)
				last++;
			(*cmd)->input[last] = (char *)-3;
		}
	}
	else if ((int)((*cmd)->process).stdin_send != -1)
	{
		ft_strjoin_free(&((*cmd)->process).stdin_send, shell->str);
		ft_strjoin_free(&((*cmd)->process).stdin_send, "\n");
	}
	ft_strdel(&shell->str);
}

/*
** La première conditon correspond au Ctrl-C dans un heredoc
** La deuxième à un Ctrl-D (si cmd existe alors il attend des heredoc)
*/

int		hrdc_fill(e_prompt *prompt, t_cmd **cmd, t_shell *shell, e_shortcut ret)
{
	if (ret == CTRLC && *prompt == PROMPT && *cmd)
	{
		return (shell_clean_data(cmd, shell, 1, 1));
	}
	if (*prompt == HRDC && ret == CTRLD && !shell->str)
	{
		ft_dprintf(2, "21sh: warning: here-document at line 84 delimited by "
				"end-of-file (wanted `%s')\n", get_next_hrdc((*cmd)->hrdc));
		while (get_next_hrdc((*cmd)->hrdc))
			del_next_hrdc((*cmd)->hrdc);
		(*cmd)->hrdc = NULL;
		*prompt = PROMPT;
		return (1);
	}
	else if (*prompt == HRDC && ret == CTRLC)
	{
		*prompt = PROMPT;
		return (shell_clean_data(cmd, shell, 1, 1));
	}
	if (*prompt == HRDC && *cmd)
	{
		hrdc_fill_stdin(prompt, cmd, shell);
		return (1);
	}
	return (0);
}
