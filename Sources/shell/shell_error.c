/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_err.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/07 19:24:29 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/07 19:24:29 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int		shell_error_env(char *msg)
{
	if (ft_strcmp(msg, "env set usage") == 0)
		dprintf(2, "shell: setenv: invalid argument\n"
						"usage: setenv VAR VALUE\n");
	else if (ft_strcmp(msg, "env unset usage") == 0)
		dprintf(2, "shell: unsetenv: invalid argument\n"
						"usage: unsetenv VAR\n");
	else if (ft_strcmp(msg, "env usage") == 0)
		dprintf(2, "shell: env: invalid argument\nusage: env VAR\n");
	else if (ft_strcmp(msg, "env $HOME not set") == 0)
		dprintf(2, "shell: $HOME env not set\n");
	else
		dprintf(2, "Error ENV : unknown msg type <%s>\n", msg);
	return (0);
}

/*
** Print un message d'erreur ou/et exit le plus proprement possible.
** {type} commence par un code suivant :
**      - env : erreur lié aux variable environement
**		- mlc : erreur de malloc. Récupère les {n} var à free avec ap puis exit
**		- gnl : fail GNL ou reception signal Ctrl-D
**
** Necessite de supprimer l'error de malloc lors malloc(32000000000000000+)
*/

int		shell_error(char *type, int n, ...)
{
	va_list	ap;

	va_start(ap, n);
	if (type[0] == 'e' && type[1] == 'n' && type[2] == 'v')
		return (shell_error_env(type));
	else if (type[0] == 'g' && type[1] == 'n' && type[2] == 'l')
	{
		ft_strdel(va_arg(ap, char **));
		ft_arrdel(va_arg(ap, char **));
		va_end(ap);
		write(1, " exit\n", 6);
		exit(1);
	}
	else if (type[0] == 'm' && type[1] == 'l' && type[2] == 'c')
	{
		ft_strdel(va_arg(ap, char **));
		ft_arrdel(va_arg(ap, char **));
		dprintf(2, "Fatal: failed to allocate %zu bytes\n", va_arg(ap, size_t));

		va_end(ap);
		exit(EXIT_FAILURE);
	}
	va_end(ap);
	return (0);
}

void	*shl_mlc(char *type, int n, ...)
{
	va_list	ap;
	void	*ret;

	va_start(ap, n);
	if (type[0] == 'c' && type[1] == 'm' && type[2] == 'd' &&
			!(ret = (void *)ft_memalloc(sizeof(t_cmd))))
		shell_error("mlc", 3, va_arg(ap, char **), va_arg(ap, char **),
					va_arg(ap, size_t));
	if (type[0] == 'a' && type[1] == 'r' && type[2] == 'g' &&
		!(ret = (void *)ft_memalloc(sizeof(t_cmd))))
		shell_error("mlc", 3, va_arg(ap, char **), va_arg(ap, char **),
					va_arg(ap, size_t));
	if (type[0] == 'a' && type[1] == 'r' && type[2] == 'g' && type[3] == 's' &&
			!(ret = (void *)ft_memalloc(va_arg(ap, size_t))))
		shell_error("mlc", 3, va_arg(ap, char **), va_arg(ap, char **),
					va_arg(ap, size_t));
	va_end(ap);
	return (ret);
}