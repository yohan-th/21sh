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
		return (EXIT_FAILURE);
	}
	if (success == 0)
	{
		ft_putstr("Terminal type `");
		ft_putstr(termtype);
		ft_putendl("' is not defined.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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
		fill_hist_file((*shell)->hist);
	ret = (*shell)->ret;
	clean_shell(shell);
	return (ret);
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
