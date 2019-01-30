/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process_input.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/01/21 15:03:02 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/21 15:03:02 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../../Include/shell.h"

int		check_fd_input(char *input, t_shell *shell)
{
	int 	fd;
	int 	i;

	if ((int)input != -3 && input[0] == '&')
	{
		shell_envpsub(&input, shell->envp);
		shl_quotesub(input);
		i = 1;
		while (ft_isdigit(input[i]))
			i++;
		if (input[i] != '\0')
			return (shell_error_prepare("ambiguous", input) - 1);
		fd = ft_atoi(input + 1);
		if (fd < 0 || fd > 2)
			return (shell_error_prepare("bad fd", input) - 1);
		else
			return (1);
	}
	return (0);
}

void 	complete_stdin_path(char **std_in, t_shell *shell)
{
	char 	*tmp;
	char 	*cur_dir;

	printf("-<std_in|%s|>\n", *std_in);
	shell_envpsub(std_in, shell->envp);
	shl_quotesub(*std_in);
	if ((*std_in)[0] != '/')
	{
		tmp = *std_in;
		cur_dir = get_cur_dir();
		*std_in = ft_strjoin_mltp(3, cur_dir, "/", *std_in);
		ft_strdel(&cur_dir);
		ft_strdel(&tmp);
	}
}

int 	check_input_file(char **std_in, t_shell *shell)
{
	if ((int)*std_in != -3)
	{
		complete_stdin_path(std_in, shell);
		if (access(*std_in, F_OK) == -1)
			return (shell_error_prepare("not found", *std_in));
		else if (ft_isdir(*std_in))
			return (shell_error_prepare("Is directory", *std_in));
		else if (access(*std_in, R_OK) == -1)
			return (shell_error_prepare("denied", *std_in));
	}
	return (1);
}

void	clean_input_to_fill_with_last(t_cmd *elem)
{
	ft_strdel(&(elem->process).fd_stdin);
	(elem->process).fd_stdin = ft_strdup("&0");
	ft_strdel(&(elem->process).stdin_send);
}

int 	shell_read_input(t_cmd *elem, t_shell *shell)
{
	int 		i;
	int 		is_fd;

	i = 0;
	while (elem->input && elem->input[i])
	{
		if (i == ft_arrlen(elem->input) - 1 && (int)elem->input[i] != -3)
			clean_input_to_fill_with_last(elem);
		if ((is_fd = check_fd_input(elem->input[i], shell)) == 1)
		{
			if (i == ft_arrlen(elem->input) - 1)
				(elem->process).fd_stdin = ft_strdup(elem->input[i]);
		}
		else if (!is_fd && check_input_file(&elem->input[i], shell))
		{
			if (i == ft_arrlen(elem->input) - 1)
				ft_read_file(elem->input[i], &(elem->process).stdin_send);
		}
		else
			return (0);
		i++;
	}
	return (1);
}
