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
** Delete les double \\ ainsi que les quotes inutiles --> tes"t $USER te"st
** Retourne 0 si les quotes ne sont pas fermé
*/

int 	shl_clean_arg(char *arg, char quote)
{
	arg += (quote == ' ') ? 0 : 1;
	while (*arg)
	{
		if (*arg == '\\' && quote != '\'')
		{
			ft_strdelchar(&arg, *arg);
			arg++;
		}
		else if (ft_strchr("'\"", *arg) && quote == ' ')
			ft_strdelchar(&arg, quote = *arg);
		else if (*arg == quote && quote != ' ' && *(arg + 1) != '\0')
		{
			ft_strdelchar(&arg, quote);
			quote = ' ';
		}
		else if (quote != ' ' && *arg == quote && *(arg + 1) == '\0')
			return (1);
		else
			arg++;
	}
	return ((quote != ' ') ? 0 : 1);
}
