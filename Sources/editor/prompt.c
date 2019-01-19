/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   prompt.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/13 23:22:07 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/19 21:32:34 by dewalter    ###    #+. /#+    ###.fr     */
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

char	*get_user_name()
{
	DIR		*dir;
	uid_t	uid;
	char	*full_path;
	struct	stat buf;
	struct	dirent *dirent;

	dir = opendir("/Users");
	uid = getuid();
	while (dir && (dirent = readdir(dir)))
	{
		full_path = build_full_path("/Users", dirent->d_name, NULL);
		if (full_path && !lstat(full_path, &buf)
		&& buf.st_uid == uid && !closedir(dir))
		{
			ft_strdel(&full_path);
			return (dirent->d_name);
		}
		ft_strdel(&full_path);
	}
	if (dir)
		closedir(dir);
	return (NULL);
}

int				display_prompt(e_prompt prompt, char **env)
{
	(void)env;
	//char			*user;
	int				len;
	struct utsname	buf;
	//uid_t			uid;

	if (prompt != PROMPT)
		return (prompt_type(prompt));
	//user = get_envp(env, "USER");
//	len = ft_putstrlen(user);
	len = ft_putstrlen(get_user_name());//42sh
	if (!uname(&buf))
	{
	//	if (user)
			len += ft_putstrlen("@");
		len += ft_putstrlen(buf.nodename);
	}
	len += ft_putstrlen("$ ");
	return (len);
}
