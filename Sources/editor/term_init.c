/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   term_init.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/08/10 02:51:08 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/17 23:20:24 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

int			term_reinit(struct termios *raw_mode)
{
	static struct termios	term_default;
	static int				state = 0;

	if (state == 0)
	{
		tcgetattr(STDIN_FILENO, &term_default);
		state = 1;
	}
	*raw_mode = term_default;
	return (0);
}

int			get_term_raw_mode(int mode)
{
	struct termios raw_mode;

	term_reinit(&raw_mode);
	if (mode)
	{
		raw_mode.c_lflag &= ~(ECHO | ICANON | ISIG);
		raw_mode.c_cflag &= ~(CSIZE | PARENB);
		raw_mode.c_cflag |= CS8;
		raw_mode.c_cc[VMIN] = 0;
		raw_mode.c_cc[VTIME] = 1;
		tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);
	}
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);
	return (0);
}

t_history	*hist_add(t_history *hist)
{
	t_history *new;
	t_history *now;

	new = NULL;
	now = NULL;
	if (!(new = (t_history*)malloc(sizeof(t_history))))
		exit (-1);
	hist->next = new;
	now = hist;
	hist = new;
	hist->cmd = NULL;
	hist->prev = now;
	hist->next = NULL;
	return (new);
}

t_editor	*line_editor_init(char **line, e_prompt prompt, int prompt_size, t_history **hist)
{
	t_editor *ed;

	if (!(ed = (t_editor *)malloc(sizeof(t_editor))))
		return (NULL);
	ed->ret = 0;
	ed->cur_col = get_cursor_position(0);
	ed->cur_row = get_cursor_position(1);
	ed->cursor_str_pos = 0;
	ed->ws_row = 0;
	ed->ws_col = 0;
	ed->first_row = ed->cur_row;
	ed->last_row = ed->first_row;
	ed->first_char = ed->cur_col;
	ed->last_char = ed->first_char;
	ed->prompt_size = prompt_size;
	ed->clipboard = NULL;
	ft_bzero(ed->key, 4);
	ed->hist = *hist && (*hist)->cmd ? hist_add(*hist) : *hist;
	if (prompt != PROMPT && prompt != E_PIPE)
		*line = ft_strjoin_free(*line, "\n");
	return (ed);
}

int			line_editor_delete(t_editor **ed, t_history **hist)
{
	int		ret;

	ret = 0;
	ret = (*ed)->ret;
	//ft_strdel(&(*ed)->line);
	ft_strdel(&(*ed)->clipboard);
	while (((*ed)->hist->next))
		(*ed)->hist = (*ed)->hist->next;
	*hist = (*ed)->hist;
	if ((*hist)->cmd)
		ft_strdel(&(*hist)->cmd);
	free(*ed);
	return (ret);
}

