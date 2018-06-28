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

void 	shell_process(t_cmd *cmd, t_shell *shell)
{
	shell->mltline = 0;
	clean_cmd(&cmd);
	if (ft_strcmp(shell->str, "exit") == 0)
	{
		ft_strdel(&shell->str);
		ft_arrdel(shell->envp);
		exit(1);
	}
	ft_strdel(&shell->str);
}