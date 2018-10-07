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

int 	get_sep(char **str)
{
	int		sep;

	*str = shell_trim(str);
	sep = 0;
	sep = (*str)[0] == '|' ? SPL_PIPE : sep;
	sep = (*str)[0] == '<' ? SPL_REDI_G : sep;
	sep = (*str)[0] == '>' ? SPL_REDI_D : sep;
	sep = (*str)[0] == ';' ? PTN_VRGL : sep;
	sep = (*str)[0] == '|' && (*str)[1] == '|' ? DBL_PIPE : sep;
	sep = (*str)[0] == '<' && (*str)[1] == '<' ? DBL_REDI_G : sep;
	sep = (*str)[0] == '>' && (*str)[1] == '>' ? DBL_REDI_D : sep;
	sep = (*str)[0] == '&' && (*str)[1] == '&' ? DBL_SPRLU : sep;
	if (sep >= 1 && sep <= 4)
		*str = *str + 1;
	else if (sep >= 5)
		*str = *str + 2;
	return (sep);
}

t_cmd	*shell_split(char *line, char **envp)
{
	t_cmd	*cmd;
	t_cmd	*prev;
	char 	**args;

	prev = NULL;
	if (!(cmd = ft_memalloc(sizeof(t_cmd))))
		shell_error("mlc", 3, &line, envp, sizeof(t_cmd));
	while ((args = get_args(&line, envp)))
	{
		printf("getargs <%s>\n", args[0]);
		cmd->args = args;
		cmd->sep = get_sep(&line);
		cmd->start = (prev) ? prev->start : cmd;
		prev = cmd;
		cmd->next_cmd = ft_memalloc(sizeof(t_cmd));
		cmd = cmd->next_cmd;
	}
	printf("prev <%p> et prev->start et strlen %d\n", prev, ft_strlen(line));
	if (ft_strlen(line) > 0)
	{
		printf("on clean cmd\n");
		clean_cmd(&cmd);
		return (NULL);
	}
	else
		return (prev ? prev->start : NULL);
}
