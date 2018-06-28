/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   strsplit_mnshl2.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/29 20:27:35 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/29 20:27:35 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"

void	clean_cmd(t_cmd **cmd)
{
	t_cmd *tmp;

	while (*cmd)
	{
		printf("delete %s\n", (*cmd)->arg);
		ft_strdel(&(*cmd)->arg);
		tmp = (*cmd);
		*cmd = (*cmd)->next_cmd;
		free(tmp);
	}
}

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
