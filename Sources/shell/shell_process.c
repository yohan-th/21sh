/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/28 20:39:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/06/28 20:39:57 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void	read_array(char **str)
{
	int i;

	i = 0;
	dprintf(2, "Read array : ");
	while (str[i])
	{
		dprintf(2, "arg[%i]=<%s> ", i, str[i]);
		i++;
	}
}

void	shell_process(t_cmd *cmd, t_shell *shell)
{
	t_cmd		*save;
	t_stdout	*read;
	int 		i;

	save = cmd;
	while ((cmd = cmd->next_cmd))
	{
		read_array(cmd->args);
		dprintf(2, "\nRead stdout : ");
		read = cmd->std_out;
		while (read != NULL)
		{
			dprintf(2, "from %d to <%s> append=%d - ", read->from, read->to, read->append);
			read = read->next;
		}
		dprintf(2, "\nRead stdin : ");
		i = 0;
		while (cmd->std_in && (cmd->std_in)[i] != NULL)
		{
			if ((int)(cmd->std_in)[i] == -1 || (int)(cmd->std_in)[i] == -2)
				dprintf(2, "<%d> -", (int)(cmd->std_in)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->std_in)[i++]);
		}
		dprintf(2, "\nRead heredoc : ");
		i = 0;
		while (cmd->hrdc && (cmd->hrdc)[i] != NULL)
		{
			if ((int)(cmd->hrdc)[i] == -1 || (int)(cmd->hrdc)[i] == -2)
				dprintf(2, "<%d> -", (int)(cmd->hrdc)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->hrdc)[i++]);
		}
		dprintf(2, "\n");
		dprintf(2, "Et sep %d\n", cmd->sep);
	}
	if (ft_strcmp(shell->str, "exit") == 0)
	{
		clean_shell(&shell);
		clean_cmd(&save);
		exit(1);
	}
	clean_cmd(&save);
	ft_strdel(&shell->str);
	//clean_shell(&shell);
}
