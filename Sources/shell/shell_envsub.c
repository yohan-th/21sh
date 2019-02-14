/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split_envsub.c                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/07 19:05:07 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/07 19:05:07 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int		shell_argsub_env(char **arg, int i, char **envp)
{
	char	*tmp;
	char	*var;
	size_t	len;

	tmp = *arg + i;
	len = 1;
	while (tmp[len] && (ft_isalnum(tmp[len]) || tmp[len] == '_'))
		len++;
	if (len == 1)
		return (i);
	tmp[0] = '\0';
	tmp = ft_strsub(tmp, 1, len - 1);
	var = get_envp(envp, tmp);
	free(tmp);
	if (var == NULL)
		tmp = ft_strjoin_mltp(2, *arg, *arg + i + len);
	else
		tmp = ft_strjoin_mltp(3, *arg, var, *arg + i + len);
	free(*arg);
	*arg = ft_strdup(tmp);
	free(tmp);
	if (var == NULL)
		return (i - 1);
	else
		return (i + ft_strlen(var) - 1);
}

void	shell_check_tilde(char **arg, char **envp)
{
	char	*tmp;

	if ((*arg)[0] == '~' && !get_envp(envp, "HOME"))
	{
		write(2, "21sh: $HOME env not set for ~\n", 30);
		return ;
	}
	if ((*arg)[0] == '~' && (*arg)[1] == '\0')
	{
		ft_strdel(arg);
		*arg = ft_strdup(ft_strdup(get_envp(envp, "HOME")));
	}
	else if ((*arg)[0] == '~' && (*arg)[1] == '/')
	{
		tmp = *arg;
		*arg = ft_strjoin_mltp(3, get_envp(envp, "HOME"), "/", *arg + 2);
		ft_strdel(&tmp);
	}
}

/*
** Remplace le tilde si existant
** Remplace les var d'environnements
*/

void	shell_envpsub(char **arg, char **envp)
{
	int		i;
	char	quote;

	shell_check_tilde(arg, envp);
	quote = *arg && ft_strchr("\"'", *arg[0]) ? (char)*arg[0] : (char)' ';
	i = (quote == ' ') ? 0 : 1;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2) && quote != '\'')
			i += 2;
		if (ft_strchr("'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '$' && quote != '\'')
			i = shell_argsub_env(arg, i, envp);
		i++;
	}
}
