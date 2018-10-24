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

#include "../Include/shell.h"

void	shell_process(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*save;
	t_redi	*read;

	save = cmd;
	while ((cmd = cmd->next_cmd))
	{
		read_array((cmd->args)->args);
		printf("\nRead redir : ");
		read = (cmd->args)->redi;
		while (read != NULL)
		{
			printf("from %d to <%s> - ", read->from, read->to);
			read = read->next;
		}
		printf("Et sep %d\n", cmd->sep);
	}
	if (ft_strcmp(shell->str, "exit") == 0)
	{
		clean_shell(&shell);
		//clean_cmd(&save);
		exit(1);
	}
	printf("clean %p\n", save);
	//clean_cmd(&save);
	printf("ok\n");
	ft_strdel(&shell->str);
	//clean_shell(&shell);
}
