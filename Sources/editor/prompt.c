/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/13 23:22:07 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/05 21:43:28 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

char			*cut_pwd_dir(char *pwd)
{
	int i;

	i = ft_strlen(pwd);
	if (pwd[0] == '/' && i == 1)
		return (pwd);
	while (i >= 0)
	{
		if (pwd[i] == '/')
			return (pwd + i + 1);
		i--;
	}
	return (0);
}

int		ft_putstrlen(char *str)
{
	return (write(1, str, ft_strlen(str)));
}

static int		prompt_type(e_prompt prompt)
{
	if (prompt == S_QUOTE)
		return (ft_putstrlen("quote> "));
	else if (prompt == B_QUOTE)
		return (ft_putstrlen("bquote> "));
	else if (prompt == D_QUOTE)
		return (ft_putstrlen("dquote> "));
	else if (prompt == E_PIPE)
		return (ft_putstrlen("pipe> "));
	else if (prompt == E_HDOC)
		return (ft_putstrlen("heredoc> "));
	else if (prompt == DB_QUOTE)
		return (ft_putstrlen("dquote bquote> "));
	return (0);
}

int		display_prompt(e_prompt prompt)
{
	unsigned char c;

	if (prompt != PROMPT)
		return (prompt_type(prompt));
	c = 0xF0;
	write(1, &c, 1);
	c = 0x9F;
	write(1, &c, 1);
	c = 0x8D;
	write(1, &c, 1);
	c = 0xAA;
	write(1, &c, 1);
	write(1, " ", 1);
	c = 0xE2;
	write(1, &c, 1);
	c = 0xA7;
	write(1, &c, 1);
	c = 0xBD;
	write(1, &c, 1);
	return (8);
}
