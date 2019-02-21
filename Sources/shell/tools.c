/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tools.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/05 23:19:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/19 14:45:03 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int			init_terminal_data(void)
{
	static char	term_buffer[2048];
	char		*termtype;
	int			success;

	if (!(termtype = getenv("TERM")))
		termtype = "xterm-256color";
	success = tgetent(term_buffer, termtype);
	if (success < 0)
	{
		ft_putstr("Could not access the termcap data base.\n");
		return (1);
	}
	if (success == 0)
	{
		ft_putstr("Terminal type `");
		ft_putstr(termtype);
		ft_putendl("' is not defined.");
		return (1);
	}
	return (0);
}

void		shell_init(t_shell **shell, e_prompt *prompt, t_cmd **cmd,
						char **env)
{
	init_terminal_data();
	*shell = init_shell(env);
	*prompt = PROMPT;
	*cmd = NULL;
}

int			shell_exit(t_cmd **cmd, t_shell **shell)
{
	int ret;

	clean_cmd(cmd);
	if ((*shell)->hist)
		fill_hist_file((*shell)->hist, ".21sh_history");
	if ((*shell)->alias)
		ft_arrdel(&(*shell)->alias);
	ret = (*shell)->ret;
	clean_shell(shell);
	return (ret);
}

char	**get_alias_from_file(char *file)
{
	t_data *lt_alias;
	t_data *lt_alias_tmp;
	char	**alias;
	char	**alias_tmp;
	int		lt_len;

	lt_len = 0;
	lt_alias = init_hist(file);
	while ((lt_alias_tmp = lt_alias) && lt_alias->prev)
		lt_alias = lt_alias->prev;
	while (lt_alias_tmp->next && ++lt_len)
		lt_alias_tmp = lt_alias_tmp->next;
	if (!(alias = ft_arrnew(lt_len)))
		exit (EXIT_FAILURE);
	alias_tmp = alias;
	while (lt_alias)
	{
		if (lt_alias->cmd)
			*alias++ = lt_alias->cmd;
		lt_alias_tmp = lt_alias->next;
		free(lt_alias);
		lt_alias = lt_alias_tmp;
	}
	alias = alias_tmp;
	return (alias);
}

t_shell		*init_shell(char **envp)
{
	t_shell *shell;

	if(!(shell = malloc(sizeof(t_shell))))
		exit (EXIT_FAILURE);
	shell->envp = init_env(envp);
	if (check_if_env_var_existing(shell->envp, "OLDPWD"))
		shell->envp = rmv_key_env(shell->envp, "OLDPWD");
	shell->str = NULL;
	shell->hrdc_tmp = NULL;
	shell->hist = init_hist(".21sh_history");
	shell->alias = get_alias_from_file(".21sh_alias");
	if (!(shell->envl = (char **)malloc(sizeof(char *))))
		exit (EXIT_FAILURE);
	shell->envl[0] = NULL;
	shell->ret = 0;
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
