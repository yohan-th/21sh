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

/*
** ATTENTION shell->str peut etre vide apres un heredoc
*/

void	shell_process(t_cmd **cmd, t_shell *shell)
{
	read_lexing((*cmd)->start);
	// prepare stdout
	// shell_envpsub(&arg, envp, quote);
	if (shell->str && ft_strcmp(shell->str, "exit") == 0)
	{
		clean_shell(&shell);
		clean_cmd(cmd);
		exit(1);
	}
	clean_cmd(cmd);
	ft_strdel(&shell->str);
	//clean_shell(&shell);
}
