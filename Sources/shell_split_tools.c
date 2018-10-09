/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   strsplit_mnshl2.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/29 20:27:35 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/29 20:27:35 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"

void	clean_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	*cmd = (*cmd)->start;
	tmp = *cmd;
	while ((*cmd = (*cmd)->next_cmd))
	{
		ft_arrdel((*cmd)->args);
		free(tmp);
		tmp = *cmd;
	}
	free(tmp);
	*cmd = NULL;
}

char	*shell_trim(char **str)
{
	int i;

	i = 0;
	while (*str && ft_isspace((*str)[i]))
		i++;
	*str = *str + i;
	return (*str);
}

int		check_last_quote(char *arg, char quote)
{
	if (quote != ' ' && *arg != quote)
		return (0);
	else if (quote != ' ' && *arg == quote)
		ft_strdelchar(&arg, quote);
	return (1);
}
