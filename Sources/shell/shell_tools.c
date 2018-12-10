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

/*
** Delete les double \\ ainsi que les quotes inutiles --> tes"t\\ $USER te"st
*/

//Fonction à Modifier
/*
void		shl_clean_arg(char *arg, char quote)
{
	arg += (quote == ' ') ? 0 : 1;
	while (*arg)
	{
		if (*arg == '\\' && quote != '\'')
		{
			ft_strdelchar(arg, *arg);
			arg++;
		}
		else if (ft_strchr("'\"", *arg) && quote == ' ')
			ft_strdelchar(&arg, quote = *arg);
		else if (*arg == quote && quote != ' ' && *(arg + 1) != '\0')
		{
			ft_strdelchar(&arg, quote);
			quote = ' ';
		}
		else
			arg++;
	}
}
*/

void		clean_shell(t_shell **shell)
{
	ft_arrdel((*shell)->envp);
	if ((*shell)->str)
		ft_strdel(&(*shell)->str);
	free(*shell);
	*shell = NULL;
}
