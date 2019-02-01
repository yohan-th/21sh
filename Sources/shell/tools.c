/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/05 23:19:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/19 18:27:38 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

/*
** Lorsqu'on lance shell, OLDPWD ne doit pas exister, on le del de dup_envp
*/

int 	check_if_env_var_existing(char **env, char*var)
{
	int i;
	char *env_var;

	i = -1;
	while (env[++i])
	{
		env_var = get_var(env[i]);
		if (env_var && !(ft_strcmp(var, env_var)))
		{
			ft_strdel(&env_var);
			return (1);
		}
		ft_strdel(&env_var);
	}
	return (0);
}

void	get_env_path_from_file(char **path, char *file)
{
	int fd;
	char *line;
	int next_path;

	line = NULL;
	next_path = 0;
	if ((fd = open(file, O_RDONLY)) > -1)
	{
		while (get_next_line(fd, &line) > 0)
		{
			if (next_path || (!next_path && *path))
				ft_strjoin_free(path, ":");
			ft_strjoin_free(path, line);
			next_path = 1;
			ft_strdel(&line);
		}
		close(fd);
	}
}

char 	**init_env_path(char **envp)
{
	char **new_envp;
	char *path;

	path = NULL;
	get_env_path_from_file(&path, "/etc/paths");
	get_env_path_from_file(&path, "/etc/paths.d/munki");
	new_envp = append_key_env(envp, "PATH", path);
	ft_strdel(&path);
	return (new_envp);
}

char 	**init_env(char **envp)
{
	int i;
	char **new_envp;
	char *var;
	char *ret;
	int j;

	i = -1;
	j = -1;
	new_envp = NULL;
	new_envp = ft_arrdup(envp);
	if (!check_if_env_var_existing(new_envp, "PATH"))
		new_envp = init_env_path(new_envp);
	if (!check_if_env_var_existing(new_envp, "SHLVL"))
		new_envp = append_key_env(new_envp, "SHLVL", "1");
	else
		while (new_envp[++i])
		{
			var = get_var(new_envp[i]);
			if (!(ft_strcmp(var, "SHLVL")))
			{
				while (new_envp[i][++j] != '=')
					;
				ret = ft_itoa(ft_atoi(new_envp[i] + j + 1) + 1);
				ft_strdel(&new_envp[i]);
				new_envp[i] = ft_strjoin_mltp(3, "SHLVL", "=", ret);
				ft_strdel(&ret);
			}
			ft_strdel(&var);
		}
	return (new_envp);
}

t_shell		*init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	shell->envp = init_env(envp);
	if (check_if_env_var_existing(shell->envp, "OLDPWD"))
		shell->envp = rmv_key_env(shell->envp, "OLDPWD");
	shell->str = NULL;
	shell->hist = init_hist();
	shell->envl = (char **)malloc(sizeof(char *));
	shell->envl[0] = NULL;
	if (!shell->hist)
		return (NULL);
	return (shell);
}

/*
** Retourne le path absolu.
** On commence avec 8 octet puis on monte puissance 2
*/

char		*get_cur_dir(void)
{
	size_t	buf;
	char	*dir;

	buf = 8;
	dir = ft_strnew(buf);
	while (getcwd(dir, buf) == NULL)
	{
		free(dir);
		buf *= 2;
		dir = ft_strnew(buf);
	}
	return (dir);
}