/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process_output_tools.c                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/01/21 23:11:59 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/21 23:11:59 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

int 	path_to_output_exist(char *output)
{
	int i;
	int exist;

	i = ft_strlen(output);
	while (i > 0)
	{
		if (output[i] == '/')
		{
			output[i] = '\0';
			break ;
		}
		i--;
	}
	exist = ft_isdir(output);
	output[i] = '/';
	return (exist);
}

/*
** Ajoute le dossier courant
*/

int		complete_output_paths(char **output_to, t_shell *shell)
{
	char 		*tmp;
	char 		*cur_dir;

	shell_envpsub(output_to, shell->envp);
	shl_quotesub(*output_to);
	if ((*output_to)[0] != '/')
	{
		cur_dir = get_cur_dir();
		tmp = *output_to;
		*output_to = ft_strjoin_mltp(3, cur_dir, "/", *output_to);
		ft_strdel(&tmp);
		ft_strdel(&cur_dir);
	}
	return (1);
}

