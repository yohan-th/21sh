/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   recup_hist_from_file.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/03 14:29:35 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/08 15:46:04 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

void		fill_hist(t_history **hist, char *line)
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

void		fill_hist_file(t_history *hist)
{
	t_history	*tmp;
	int			fd;

	if ((fd = open(".21sh_history", O_WRONLY | O_CREAT |
	O_APPEND | O_TRUNC, 0644)) < 0)
		return ;
	while (hist->prev)
		hist = hist->prev;
	while (hist)
	{
		if (hist->cmd)
		{
			write(fd, hist->cmd, (size_t)ft_strlen(hist->cmd));
			ft_strdel(&hist->cmd);
			write(fd, "\n", 1);
		}
		tmp = hist->next;
		free(hist);
		hist = tmp;
	}
	free(hist);
	close(fd);
}

t_history	*recup_hist_from_file(t_history *hist, int fd)
{
	char	c;
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
			c = c ? 0 : buf[0];
	}
	return (hist);
}
/*
char	cmd_found(t_history **hist, int )
{
	char *tmp;

	tmp = NULL;

}
*/
/*
int		check_quote(char *buf, char c, int i)
{
	if (((buf[i] == '\'' || buf[i] == '\"' || buf[i] == '`') && c == 0)
	|| ((c == '\'' && buf[i] == '\'') || (c == '\"' && buf[i] == '\"')
	|| (c == '`' && buf[i] == '`')))
		return (1);
	return (0);
}

void	add_cmd_to_hist(t_history **hist, char **cmd, char **tmp)
{
	if (*cmd)
	{
		ft_strjoin_free(cmd, *tmp);
		ft_strdel(tmp);
	}
	else
		*cmd = *tmp;
	(*hist)->cmd = ft_strdup(*cmd);
	*hist = hist_add(*hist);
	ft_strdel(cmd);
}

void	recup_cmd_from_buf(t_history **hist, char *buf)
{
	int i;
	int j;
	char c;
	char *cmd;
	char *tmp;

	cmd = NULL;
	tmp = NULL;
	i = 0;
	j = 0;
	c = 0;
	while (buf[i])
	{
		if (buf[i] == '\n' && c == 0)
		{
			if (i - j)
				tmp = ft_strsub(buf, j, i - j);
			add_cmd_to_hist(hist, &cmd, &tmp);*/
/*			if (cmd)
			{
				ft_strjoin_free(&cmd, tmp);
				ft_strdel(&tmp);
			}
			else
				cmd = tmp;
			(*hist)->cmd = ft_strdup(cmd);
			*hist = hist_add(*hist);
			ft_strdel(&cmd);*//*
			j = i + 1;
		}
		if (check_quote(buf, c, i))
			c = c ? 0 : buf[i];
		if ((buf[i] != '\n' && buf[i + 1] == '\0') || (c && buf[i] == '\n' && buf[i + 1] == '\0'))
		{
			tmp = ft_strsub(buf, j, i + 1);
			ft_strjoin_free(&cmd, tmp);
			ft_strdel(&tmp);
		}
		i++;
	}
}
int		recup_hist_from_file(t_history **hist, int fd)
{*/
/*	int		i;
	int		j;
	char	c;*//*
	int		ret;
	char	buf[BUFF_READ + 1];
	char	*cmd;
	char	*tmp;

	cmd = NULL;
	tmp = NULL;
	c = 0;
	ret = 1;
	dprintf(2, "buf: %d\n", BUFF_READ);
	while (ret > 0)
	{
		ft_bzero(buf, BUFF_READ + 1);
		ret = read(fd, buf, BUFF_READ);
		recup_cmd_from_buf(hist, buf);*/
/*		i = 0;
		j = 0;
		while (buf[i])
		{
			if (buf[i] == '\n' && c == 0)
			{
				if (i - j)
					tmp = ft_strsub(buf, j, i - j);
				if (cmd)
				{
					ft_strjoin_free(&cmd, tmp);
					ft_strdel(&tmp);
				}
				else
					cmd = tmp;
				(*hist)->cmd = ft_strdup(cmd);
				*hist = hist_add(*hist);
				ft_strdel(&cmd);
				j = i + 1;
			}
			if (check_quote(buf, c, i))
				c = c ? 0 : buf[i];
			if ((buf[i] != '\n' && buf[i + 1] == '\0') || (c && buf[i] == '\n' && buf[i + 1] == '\0'))
			{
				tmp = ft_strsub(buf, j, i + 1);
				ft_strjoin_free(&cmd, tmp);
				ft_strdel(&tmp);
			}
			i++;
		}*//*
	}
	return (ret);
}*/

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
