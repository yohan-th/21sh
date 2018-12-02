/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_hrdc_check.c                               .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/02 17:55:46 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/02 17:55:46 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

void	hrdc_write(char **hrdc, t_shell *shl, t_cmd *cmd)
{
	e_prompt	prompt;
	char 		*input;
	//char 		*hrdc_input;

	prompt = HRDC;
	input = NULL;
	ft_arrdel(cmd->hrdc);
	while (get_stdin(&input, &prompt, &shl->hist, shl->envp) != -2)
	{
		if (input)
		{

			printf("input = <%s>\n", input);
		}
	}
}

BOOL	hrdc_check(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*next;

	next = cmd;
	while ((next = next->next_cmd))
	{
		if (next->hrdc)
		{
			//hrdc_write(next->hrdc, shell, next);
			return (1);
		}
	}
	return (0);
}


