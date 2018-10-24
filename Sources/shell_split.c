/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/30 21:18:39 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/06/30 21:18:39 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"


int		get_sep(char **str)
{
	int		sep;

	*str = shell_trim(str);
	sep = 0;
	sep = (*str)[0] == '|' ? SPL_PIPE : sep;
	sep = (*str)[0] == ';' ? PTN_VRGL : sep;
	sep = (*str)[0] == '|' && (*str)[1] == '|' ? DBL_PIPE : sep;
	sep = (*str)[0] == '&' && (*str)[1] == '&' ? DBL_SPRLU : sep;
	if (sep >= 1 && sep <= 2)
		*str = *str + 1;
	else if (sep >= 3)
		*str = *str + 2;
	return (sep);
}

/*
** Split line jusqu'au prochain delimiteur
** Le premier maillon start pointé par tous les autres est vide.
** ça permet de garder la logique du dernier maillon qui est rempli et pointe
** next_cmd sur NULL sinon c'est le dernier maillon qui sera vide
*/

t_cmd	*shell_split(char *line, char **envp)
{
	t_cmd	*cmd;
	t_args	*args;

	cmd = (t_cmd *)shl_mlc("cmd", 3, &line, envp, sizeof(t_cmd));
	cmd->start = cmd;
	while ((args = get_args(&line, envp)))
	{
		cmd->next_cmd = (t_cmd *)shl_mlc("cmd", 3, &line, envp, sizeof(t_cmd));
		(cmd->next_cmd)->start = cmd->start;
		cmd = cmd->next_cmd;
		cmd->args = args;
		cmd->sep = get_sep(&line);
		cmd->next_cmd = NULL;
	}

	if (ft_strlen(line) > 0)
	{
		clean_cmd(&cmd);
		return (NULL);
	}
	else
		return (cmd->start);
}
