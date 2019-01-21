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

int		check_fd_output(char *output, t_shell *shell)
{
	int 	fd;
	int 	i;

	if (output[0] == '&')
	{
		shell_envpsub(&output, shell->envp);
		shl_quotesub(output);
		i = 1;
		while (ft_isdigit(output[i]))
			i++;
		if (output[i] != '\0')
			return (shell_error_prepare("ambiguous", output) - 1);
		fd = ft_atoi(output + 1);
		if (fd < 0 || fd > 2)
			return (shell_error_prepare("bad fd", output) - 1);
		else
			return (1);
	}
	return (0);
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

int 	is_recheable_output(t_output *output, t_shell *shell)
{
	int fd_open;

	fd_open = 0;
	complete_output_paths(&output->to, shell);
	if (ft_isdir(output->to))
		return (shell_error_prepare("Is directory", output->to));
	else if (!path_to_output_exist(output->to))
		return (shell_error_prepare("not found", output->to));
	else if (access(output->to, F_OK) == 0 && access(output->to, W_OK) == -1)
		return (shell_error_prepare("denied", output->to));
	else if (access(output->to, F_OK) == -1)
	{
		if (output->append)
			fd_open = open(output->to, O_WRONLY | O_APPEND);
		else
			fd_open = open(output->to, O_WRONLY | O_CREAT);
	}
	return (fd_open);
}

void 	shell_set_output_file(t_output *output, t_cmd *elem, int fd_file)
{
		if (output->from == 1)
		{
			(elem->process).fd_stdout = ft_strdup(output->to);
			(elem->process).fd_fileout = fd_file;
		}
		else if (output->from == 2)
		{
			(elem->process).fd_stderr = ft_strdup(output->to);
			(elem->process).fd_fileerr = fd_file;
		}
}

/*
** complete_output fail si $HOME n'existe pas (getpwnam interdit)
*/

int		shell_set_output(t_cmd *elem, t_shell *shell)
{
	t_output	*output;
	int 		is_fd;
	int 		fd_file;

	output = elem->output;
	while (output != NULL)
	{
		if (output->from == 1 && (elem->process).fd_stdout)
			ft_strdel(&(elem->process).fd_stdout);
		else if (output->from == 2 && (elem->process).fd_stderr)
			ft_strdel(&(elem->process).fd_stderr);
		if (((is_fd = check_fd_output(output->to, shell)) == 1))
		{
			if (output->from == 1)
				(elem->process).fd_stdout = ft_strdup(output->to);
			else if (output->from == 2)
				(elem->process).fd_stderr = ft_strdup(output->to);
		}
		else if (!is_fd && (fd_file = is_recheable_output(output, shell)))
			shell_set_output_file(output, elem, fd_file);
		else
			return (0);
		output = output->next;
	}
	return (1);
}
