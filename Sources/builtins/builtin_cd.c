/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin_cd.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/06 23:35:03 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/06 23:35:03 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void	cd_change_env(char ***envp, char *pwd, char *old_pwd, char *dir)
{
	if (!pwd || access(pwd, X_OK) == -1)
	{
		write(2, "21sh: cd: ", 10);
		if (!pwd)
			write(2, "$HOME env not set\n", 18);
		else if (access(pwd, F_OK) == 0 && access(pwd, X_OK) == -1)
		{
			write(2, pwd, (size_t)ft_strlen(pwd));
			write(2, ": Permission denied\n", 20);
		}
		else if (dir)
		{
			write(2, dir, (size_t)ft_strlen(dir));
			write(2, ": No such directory\n", 20);
		}
	}
	else
	{
		chdir(pwd);
		builtin_setenv(envp, "PWD", pwd);
		builtin_setenv(envp, "OLDPWD", old_pwd);
	}
}

/*
** {tmp} doit terminer par "/"
*/

int		cd_move(char ***envp, char *cur_dir, char *dir)
{
	char	*tmp;

	if (ft_strchr(dir, '~') && !get_envp(*envp, "HOME"))
		return (shell_error("env $HOME not set", 0));
	else if (dir[0] == '~')
		tmp = ft_strjoin_mltp(3, get_envp(*envp, "HOME"), dir + 1, "/");
	else if (dir[0] == '/')
		tmp = ft_strjoin(dir, "/");
	else
		tmp = ft_strjoin_mltp(4, cur_dir, "/", dir, "/");
	cd_change_env(envp, tmp, cur_dir, dir);
	ft_strdel(&tmp);
	return (1);
}

/*
** Supprime les doublons de / et .
** /fld1///fld2/././fld3 --> /fld1/fld2/fld3
*/

void	cd_clean_path(char *pwd)
{
	int		i;
	char	*ptn;

	i = 0;
	while (pwd && pwd[i])
	{
		ptn = pwd + i;
		if (pwd[i] == '/' && pwd[i + 1] == '/')
			ft_strdelchar(&ptn, '/');
		else if (pwd[i] == '/' && pwd[i + 1] == '.' && (i + 2) < ft_strlen(pwd)
				&& pwd[i + 2] == '/')
		{
			ft_strdelchar(&ptn, '/');
			ft_strdelchar(&ptn, '.');
		}
		else
			i++;
	}
	i--;
	while (pwd && ft_strchr("./ ", pwd[i]) && i > 0)
		pwd[i--] = '\0';
}

char	*cd_cur_dir(char **cmd, char ***envp, int *i)
{
	char	*cur_dir;

	*i = 1;
	while (cmd[*i] != NULL && cmd[*i][0] == '-' && cmd[*i][1] != '\0')
		(*i)++;
	if ((cmd[(*i) - 1][0] == '-' &&
		cmd[(*i) - 1][ft_strlen(cmd[(*i) - 1]) - 1] == 'P') ||
		get_envp(*envp, "PWD") == NULL)
		cur_dir = get_cur_dir();
	else
		cur_dir = ft_strdup(get_envp(*envp, "PWD"));
	return (cur_dir);
}

/*
** Par default cd suit les liens (option -L)
*/

int		builtin_cd(char **cmd, char ***envp)
{
	int		i;
	char	*cur_dir;
	int		ret;

	ret = 0;
	cur_dir = cd_cur_dir(cmd, envp, &i);
	if (cmd[i] == NULL || ft_strlen(cmd[i]) == 0)
		cd_change_env(envp, get_envp(*envp, "HOME"), cur_dir, "HOME");
	else if (ft_strcmp(cmd[i], "-") == 0 && get_envp(*envp, "OLDPWD") == NULL)
	{
		write(2, "21sh: cd: OLDPWD not set\n", 25);
		ret = 0;
	}
	else if (ft_strcmp(cmd[i], "-") == 0)
		cd_change_env(envp, get_envp(*envp, "OLDPWD"), cur_dir, "OLDPWD");
	else if (ft_strcmp(cmd[i], "..") == 0 && ft_strcmp(cur_dir, "/") != 0)
		cd_change_env(envp, cd_rmv_last_path(cur_dir), cur_dir, "RMV_LAST");
	else
		ret = cd_move(envp, cur_dir, cmd[i]);
	cd_clean_path(get_envp(*envp, "PWD"));
	ft_strdel(&cur_dir);
	return (ret);
}
