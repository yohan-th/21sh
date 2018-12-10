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

void	clean_hrdc(char **arr)
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

void	clean_redi(t_stdout **redi)
{
	t_stdout	*tmp;

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

	if (!(*cmd))
	{
		printf("[!] <erreur de clean cmd>\n");
		return (0);
	}
	*cmd = (*cmd)->start;
	tmp = *cmd;
	while ((*cmd = (*cmd)->next_cmd))
	{
		ft_arrdel((*cmd)->args);
		clean_hrdc((*cmd)->hrdc);
		ft_strdel(&(*cmd)->hrdc_stdin);
		if ((*cmd)->std_out)
			clean_redi(&((*cmd)->std_out));
		free(tmp);
		tmp = *cmd;
	}
	free(tmp);
	*cmd = NULL;
	return (1);
}
