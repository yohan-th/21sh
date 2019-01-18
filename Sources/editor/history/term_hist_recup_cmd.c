/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   recup_hist_from_file.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/03 14:29:35 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/18 15:48:57 by dewalter    ###    #+. /#+    ###.fr     */
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

t_history	*recup_hist_from_file(t_history *hist, char *file)
{
	char	c;
	int		i;
	int		j;

	c = 0;
	i = 0;
	j = 0;
	while (file && file[i])
	{
		if (file[i] == '\n' && c == 0)
		{
			hist->cmd = ft_strsub(file, j, i - j);
			hist = hist_add(hist);
			j = i + 1;
		}
		if (((file[i] == '\'' || file[i] == '\"' || file[i] == '`') && c == 0)
			|| ((c == '\'' && file[i] == '\'') || (c == '\"' && file[i] == '\"')
			|| (c == '`' && file[i] == '`')))
			c = c ? 0 : file[i];
		i++;
	}
	return (hist);
}

t_history	*init_hist(void)
{
	t_history	*hist;
	char		*file;
	int			fd;
	int			ret;

	hist = malloc(sizeof(t_history));
	hist->cmd = NULL;
	hist->next = NULL;
	hist->prev = NULL;
	file = NULL;
	ret = 0;
	if (!access(".21sh_history", R_OK | F_OK))
		if ((fd = open(".21sh_history", O_RDONLY)))
		{
			if ((ret = get_read_key(fd, &file)))
			{
				hist = recup_hist_from_file(hist, file);
				ft_strdel(&file);
			}
			close(fd);
		}
	if (ret == -1)
		return (NULL);
	return (hist);
}
