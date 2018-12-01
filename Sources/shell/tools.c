/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/05 23:19:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/01 14:46:42 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

/*
** Lorsqu'on lance shell, OLDPWD ne doit pas exister, on le del de dup_envp
*/

t_history	*recup_hist_from_file(t_history *hist, int fd)
{
	int		c;
	char	buf[2];
	char	*cmd;

	cmd = NULL;
	c = 0;
	while (read(fd, buf, 1) > 0)
	{
		buf[1] = '\0';
		if (buf[0] == '\n' && c == 0 && cmd)
		{
			hist->cmd = ft_strdup(cmd);
			hist = hist_add(hist);
			ft_strdel(&cmd);
			c = 0;
		}
		else
			ft_strjoin_free(&cmd, buf);
		if (((buf[0] == '\'' || buf[0] == '\"' || buf[0] == '`') && c == 0)
			|| ((c == '\'' && buf[0] == '\'') || (c == '\"' && buf[0] == '\"')
			|| (c == '`' && buf[0] == '`')))
			c = (c) ? 0 : buf[0];
	}
	return (hist);
}

t_history	*init_hist(void)
{
	t_history	*hist;
	int			fd;

	hist = malloc(sizeof(t_history));
	hist->cmd = NULL;
	hist->next = NULL;
	hist->prev = NULL;
	if (!access(".21sh_history", R_OK | F_OK))
		if ((fd = open(".21sh_history", O_RDONLY)))
		{
			hist = recup_hist_from_file(hist, fd);
			close(fd);
		}
	return (hist);
}

t_shell		*init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	shell->envp = ft_arrdup(envp);
	shell->envp = rmv_key_env(shell->envp, "OLDPWD");
	shell->str = NULL;
	shell->hist = init_hist();
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
