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

#include "../../Include/shell.h"

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

BOOL	checkredi_to(t_stdout *redis)
{
	t_stdout	*read;

	read = redis;
	while (read != NULL)
	{
		if (read->to == NULL)
			return (0);
		read = read->next;
	}
	return (1);
}

/*
** Split line jusqu'au prochain delimiteur
** Le premier maillon start (pointé par tous les autres) est vide.
*/

t_cmd	*shell_split(char *line, char **envp, e_prompt *prompt)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)shl_mlc("cmd", 3, &line, envp, sizeof(t_cmd));
	cmd->start = cmd;
	while (line && (cmd->next_cmd = get_args(&line, envp, prompt)))
	{
		(cmd->next_cmd)->start = cmd->start;
		cmd = cmd->next_cmd;
		cmd->sep = get_sep(&line);
		if ((!ft_strlen(cmd->args[0]) && cmd->sep) || !checkredi_to(cmd->std_out))
			break ;
	}
	if ((!line || ft_strlen(line)) && ft_strlen(cmd->args[0]) && !cmd->sep)
	{
		clean_cmd(&cmd);
		return (NULL);
	}
	else
	{
		*prompt = PROMPT;
		return (cmd->start);
	}
}
