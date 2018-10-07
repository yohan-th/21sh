/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split_getarg.c                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/07 18:55:09 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/07 18:55:09 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"

int		shl_quotesub(char *arg)
{
	char	quote;

	quote = ft_strchr("'\"`", *arg) ? *arg : (char)' ';
	if (quote != ' ')
		ft_strdelchar(&arg, quote);
	while (*arg)
	{
		if (*arg == '\\' && quote != '\'')
		{
			ft_strdelchar(&arg, *arg);
			arg++;
		}
		else if (ft_strchr("'\"`", *arg) && quote == ' ')
			ft_strdelchar(&arg, quote = *arg);
		else if (*arg == quote && quote != ' ')
		{
			ft_strdelchar(&arg, quote);
			quote = ' ';
		}
		else
			arg++;
	}
	return (check_last_quote(arg, quote));
}

/*
** Calcul la longueur de l'argument
** Les conditions de fin sont :
**  - match de nouveau le charactere de {quote}
**  - match avec {;} OU {&&} OU {|} OU {<} OU {>}
*/

size_t	len_arg(char *str, char quote)
{
	size_t i;

	if (ft_strchr(";&|", str[0]))
		return (ft_strchr("&|", str[1]) ? 2 : 1);
	i = 0;
	while (*str && str[i])
	{
		i++;
		if (str[i - 1] == '\\' && str[i] == quote && quote != '\'')
			i++;
		if (ft_strchr("'\"`", str[i]) && quote == ' ')
			quote = str[i++];
		if (str[i] == quote && quote != ' ' && (!ft_strchr("\0 ", str[i + 1])))
			quote = ' ';
		if (quote == ' ' && str[i - 1] != '\\' && (ft_strchr(";|<>", str[i]) ||
												   (str[i] == '&' && str[i + 1] == '&')))
			break ;
		if (str[i] == quote && (quote == ' ' || ft_strchr("\0 ", str[i + 1])))
			break ;
	}
	return (i);
}

int 	get_nb_args(char *str)
{
	char	quote;
	size_t 	len;
	int 	nb_args;

	nb_args = 0;
	while (*str && !ft_strchr(";|&><", *str))
	{
		str = shell_trim(&str);
		quote = ft_strchr("'\"`", *str) ? *str : (char)' ';
		str += len_arg(str, quote);
		nb_args += 1;
		str = shell_trim(&str);
	}
	return (nb_args);
}

/*
** subtitue le premier args de str et avance str de la len
** On profite d'avoir les quotes pour remplacer les var env
*/

char	*get_arg(char **str, char **envp)
{
	unsigned int	i;
	char			quote;
	char			*arg;

	i = 0;
	while (*str && ft_isspace((*str)[i]))
		i++;
	if (!(*str) || (*str)[i] == '\0')
		return (0);
	quote = ft_strchr("'\"`", (*str)[i]) ? (*str)[i] : (char)' ';
	arg = ft_strsub(*str, i, len_arg(*str + i, quote));
	if ((arg)[0] == '~' && arg[1] == '\0')
	{
		ft_strdel(&arg);
		if (get_envp(envp, "HOME") == NULL)
			arg = ft_strnew(0);
		else
			arg = ft_strdup(get_envp(envp, "HOME"));
	}
	shell_envpsub(&arg, envp);
	if (!(shl_quotesub(arg)))
		ft_strdel(&arg);
	else
		*str = *str + i + len_arg(*str + i, quote);
	return (arg);
}

char 	**get_args(char **line, char **envp)
{
	char 	**args;
	size_t	i;
	char 	quote;
	int 	nb_args;

	if (!(*line) || (*line)[0] == '\0')
		return (NULL);
	nb_args = get_nb_args(*line);
	if (!(args = (char **)malloc(sizeof(char *) * (nb_args + 1))))
		shell_error("mlc", 3, line, envp, (sizeof(char *) * (nb_args + 1)));
	args[nb_args] = NULL;
	i = 0;
	while (i < nb_args)
		args[i++] = get_arg(line, envp);
	return (args);
}
