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

void	check_fd_devnull(char **ptn_output, int fd_devnull)
{
	char *tmp;

	if (ft_strcmp(*ptn_output, "&-") == 0)
	{
		ft_strdel(ptn_output);
		tmp = ft_itoa(fd_devnull);
		*ptn_output = ft_strjoin("&", tmp);
		ft_strdel(&tmp);
	}
}


int		path_to_output_exist(char *output)
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

int		path_to_output_recheable(char *output)
{
	int	i;
	int recheable;

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
	recheable = access(output, W_OK);
	if (recheable == 0)
		output[i] = '/';
	return (recheable);
}

/*
** Ajoute le dossier courant
*/

int		complete_output_paths(char **output_to, t_shell *shell)
{
	char *tmp;
	char *cur_dir;

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
