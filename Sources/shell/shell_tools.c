/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_tools.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/18 15:24:03 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/18 15:24:03 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void		shl_quotesub(char *arg)
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
}

void		clean_shell(t_shell **shell)
{
	ft_arrdel((*shell)->envp);
	if ((*shell)->str)
		ft_strdel(&(*shell)->str);
	free(*shell);
	*shell = NULL;
}

void	shell_save_fd(int fd[3])
{
	fd[0] = dup(0);
	fd[1] = dup(1);
	fd[2] = dup(2);
}

void	reinit_fd(int fd[3])
{
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	dup2(fd[2], 2);
}
