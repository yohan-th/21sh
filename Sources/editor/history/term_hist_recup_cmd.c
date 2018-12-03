/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   recup_hist_from_file.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/03 14:29:35 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/03 14:59:04 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void	fill_hist(t_history **hist, char *line)
{
	t_history *new;
	t_history *now;

	new = NULL;
	(*hist)->next = new;
	now = *hist;
	*hist = new;
	(*hist)->cmd = ft_strdup(line);
	(*hist)->prev = now;
	(*hist)->next = NULL;
}

void	fill_hist_file(t_history *hist)
{
	int fd;

	if ((fd = open(".21sh_history", O_WRONLY | O_CREAT |
	O_APPEND | O_TRUNC, 0644)) < 0)
		return ;
	while (hist)
	{
		if (hist->cmd)
		{
			write(fd, hist->cmd, (size_t)ft_strlen(hist->cmd));
			write(fd, "\n", 1);
		}
		hist = hist->prev;
	}
	close(fd);
}

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
