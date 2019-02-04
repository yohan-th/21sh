/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_clean.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/10 15:33:13 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/10 15:33:13 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void	clean_arr_mlti(char **arr)
{
	int i;

	i = 0;
	while (arr != NULL && arr[i] != NULL)
	{
		if ((int)arr[i] == -1 || (int)arr[i] == -2 || (int)arr[i] == -3)
			arr[i++] = NULL;
		else
			ft_strdel(&arr[i++]);
	}
	if (arr != NULL)
		free(arr);
}

void	clean_redi(t_output **redi)
{
	t_output	*tmp;

	while ((*redi)->next != NULL)
	{
		tmp = *redi;
		(*redi) = (*redi)->next;
		ft_strdel(&tmp->to);
		free(tmp);
	}
	ft_strdel(&(*redi)->to);
	free(*redi);
}

int		clean_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	if (cmd && *cmd)
	{
		*cmd = (*cmd)->start;
		tmp = *cmd;
		while ((*cmd = (*cmd)->next_cmd))
		{
			if ((*cmd)->args)
				ft_arrdel(&(*cmd)->args);
			clean_arr_mlti((*cmd)->hrdc);
			clean_arr_mlti((*cmd)->input);
			ft_strdel(&(*cmd)->process.fd_stdin);
			ft_strdel(&(*cmd)->process.fd_stdout);
			ft_strdel(&(*cmd)->process.fd_stderr);
			ft_strdel(&(*cmd)->process.stdin_send);
			ft_strdel(&(*cmd)->exec);
			if ((*cmd)->output)
				clean_redi(&((*cmd)->output));
			free(tmp);
			tmp = *cmd;
		}
		free(tmp);
		*cmd = NULL;
	}
	return (1);
}

int 	shell_clean_data(t_cmd **cmd, t_shell *shell, BOOL t_cmd, BOOL shl_str)
{
	if (t_cmd)
		clean_cmd(cmd);
	if (shl_str)
		ft_strdel(&shell->str);
	return (1);
}
