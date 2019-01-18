/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/13 23:22:07 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/18 22:14:13 by dewalter    ###    #+. /#+    ###.fr     */
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

static int		ft_putstrlen(char *str)
{
	return (write(1, str, ft_strlen(str)));
}

static int		prompt_type(e_prompt prompt)
{
	if (prompt == B_QUOTE)
		return (ft_putstrlen("bquote>"));
	else if (prompt == S_QUOTE)
		return (ft_putstrlen("quote> "));
	else if (prompt == D_QUOTE)
		return (ft_putstrlen("dquote> "));
	else if (prompt == HRDC)
		return (ft_putstrlen("heredoc> "));
	return (0);
}

int				display_prompt(e_prompt prompt, char **env)
{
	char	*user;
	int		len;
	struct	utsname buf;

	if (prompt != PROMPT)
		return (prompt_type(prompt));
	user = get_envp(env, "USER");
	len = ft_putstrlen(user);
	if (!uname(&buf))
	{
		if (user)
			len += ft_putstrlen("@");
		len += ft_putstrlen(buf.nodename);
	}
	len += ft_putstrlen("$ ");
	return (len);
}
