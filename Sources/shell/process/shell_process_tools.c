/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process_tools.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/11 01:01:54 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/11 01:01:54 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

void	shell_prcs_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
}
