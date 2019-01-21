/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process_output.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/01/21 15:02:55 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/21 15:02:55 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

/*
** Ajoute le dossier courant
*/

int		complete_stdout_paths(t_output *output, t_shell *shell)
{
	char 		*tmp;
	t_output 	*elem;
	char 		*cur_dir;

	elem = output;
	while (elem != NULL)
	{
		shell_envpsub(&elem->to, shell->envp);
		shl_quotesub(elem->to);
		if (elem->to[0] != '/' && elem->to[0] != '&')
		{
			cur_dir = get_cur_dir();
			tmp = elem->to;
			elem->to = ft_strjoin_mltp(3, cur_dir, "/", elem->to);
			ft_strdel(&tmp);
			ft_strdel(&cur_dir);
		}
		elem = elem->next;
	}
	return (1);
}

int 	check_output_recheable(t_output *output)
{
	t_output 	*elem;

	elem = output;
	while (elem != NULL)
	{
		if (elem->to && elem->to[0] != '&')
		{
			if (ft_isdir(elem->to))
				return (shell_error_prepare("Is directory", elem->to));
			else if (access(elem->to, R_OK) == -1)
				return (shell_error_prepare("denied", elem->to));
		}
		else if (elem->to && elem->to[0] == '&')
		{
			if (ft_atoi(elem->to + 1) >= 0 && ft_atoi(elem->to + 1) <= 2)
				return (shell_error_prepare("bad fd", elem->to + 1));
		}

		elem = elem->next;
	}
	return (1);
}

/*
** complete_stdout fail si $HOME n'existe pas et ~ appellé (getpwnam interdit)
*/

int		shell_set_output(t_cmd *elem, t_shell *shell)
{
	t_output	*std_out;

	std_out = elem->output;
	while (std_out != NULL)
	{
		complete_stdout_paths(std_out, shell);
		check_output_recheable(std_out);

		//	if (std_out->to[0] == '&' && ft_atoi(std_out->to + 1) == 1)
		//	elem->process.file_stdout = std_out->append ?
		//			open(std_out->to, O_WRONLY | O_APPEND) :
		//			open(std_out->to, O_WRONLY | O_CREAT);


		//}
		std_out = std_out->next;
	}
	return (1);
}
