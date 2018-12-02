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

#include "../../Include/shell.h"

int		check_last_quote(char *arg, char quote)
{
	if (quote != ' ' && *arg != quote)
		return (0);
	else if (quote != ' ' && *arg == quote)
		ft_strdelchar(&arg, quote);
	return (1);
}

int		shl_quotesub(char *arg)
{
	char	quote;

	quote = ft_strchr("'\"", *arg) ? *arg : (char)' ';
	if (quote != ' ')
		ft_strdelchar(&arg, quote);
	while (*arg)
	{
		if (*arg == '\\' && quote != '\'')
		{
			ft_strdelchar(&arg, *arg);
			arg++;
		}
		else if (ft_strchr("'\"", *arg) && quote == ' ')
			ft_strdelchar(&arg, quote = *arg);
		else if (*arg == quote && quote != ' ')
		{
			ft_strdelchar(&arg, quote);
			quote = ' ';
		}
		else
			arg++;
	}
	return (check_last_quote(arg, quote));
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
		if ((*cmd)->std_out)
			clean_redi(&((*cmd)->std_out));
		free(tmp);
		tmp = *cmd;
	}
	free(tmp);
	*cmd = NULL;
	return (1);
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
