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

#include "../Include/shell.h"

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

void	shell_envpsub(char **arg, char **envp)
{
	char	quote;
	int		i;

	i = 0;
	quote = ft_strchr("'\"", (*arg)[i]) ? (*arg)[i] : (char)' ';
	while ((*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2))
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
