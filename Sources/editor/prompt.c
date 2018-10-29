/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/13 23:22:07 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/24 19:13:24 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

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
	char	pwd[4096];
//	char	*error;
//	char	*home;
	int		len;

	len = 0;
//	home = prompt == 0 ? ft_getenv("HOME", g_env) : NULL;
	if (prompt != PROMPT)
		return (prompt_type(prompt));
	getcwd(pwd, sizeof(pwd));
//	error = ft_getenv("?", g_set);
//	ft_putstr(error && error[0] == '1' ? "\e[91m➜\e[0m" : "\e[92m➜\e[0m");
	len += ft_putstrlen("  \e[1m\e[93m");
//	if (!ft_strcmp(pwd, !home ? "" : home))
//		ft_putstr("~");
//	else
	len += ft_putstrlen(cut_pwd_dir(pwd));
	len += ft_putstrlen("\e[0m ");
	//ft_print_git_branch();
//	ft_putstr(error && error[0] == '1' ? "\U0001F44E" : "\U0001F44D");
	len += ft_putstrlen("  ");
//	free(error);
//	ft_strdel(&home);
	return (len);
}
