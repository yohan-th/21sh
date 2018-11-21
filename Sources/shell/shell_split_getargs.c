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

#include "../../Include/shell.h"

BOOL	check_quote(char *str, char quote, size_t i)
{
	if (!str)
		return(false);
	i -= 1;
	if (ft_strchr("\"'", quote) && i == 0)
		return (false);
	if (quote == '"' && (str[i] != quote ||
			(str[i] == quote && str[i - 1] == '\\')))
		return (false);
	if (quote == '\'' && str[i] != quote)
		return (false);
	return (true);
}

/*
** Calcul la longueur de l'argument
** Les conditions de fin sont :
**  - match de nouveau le charactere de {quote}
**  - match avec {;} OU {&&} OU {|}
*/

size_t	len_arg(char *str, char quote)
{
	size_t i;

	i = (quote == ' ') ? 0 : 1;
	while (str && str[i])
	{
		if (str[i] == '\\' && ft_strlen(str) > (i + 1) && quote == ' ')
			i += 2;
		if (ft_strchr("'\"", str[i]) && quote == ' ')
			quote = str[i++];
		else if (str[i] == quote && quote != ' ' &&
				(!ft_strchr("\0 ", str[i + 1])))
			quote = ' ';
		else if ((quote == ' ' && (ft_strchr(";|", str[i]) || (str[i] == '&' &&
				str[i + 1] == '&'))) || (str[i] == quote && (quote == ' ' ||
				ft_strchr("\0 ", str[i + 1]))))
			break ;
		else
			i++;
	}
	/*if (!check_quote(str, quote, i))
		printf("<len arg = 0 error de quote>\n");
	else
		printf("<len arg = %zu>\n", i);*/
	return (check_quote(str, quote, i) ? i : 0);
}

int 	get_nbarg(char *str, e_prompt *prompt)
{
	char	quote;
	int 	nb_args;
	size_t	lenarg;

	nb_args = 0;
	while (*str)
	{
		str = shell_trim(&str);
		quote = ft_strchr("'\"", *str) ? *str : (char)' ';
		lenarg = len_arg(str, quote);
		str += lenarg;
		if (lenarg == 0 && !ft_strchr(";|", str[0]) && str[0] != '&' &&
															 str[1] != '&')
			return (0);
		nb_args += 1;
		str = shell_trim(&str);
		if (ft_strchr(";|", str[0]) || (str[0] == '&' && str[1] == '&'))
			break ;
	}
	return (nb_args);
}

/*
** subtitue le premier {args_tab} de {str} et avance de {len} de {args_tab}.
** {arg} garde les quotes pour {shell_envsub} au moment du [process].
** /!\ shell_redi n'est pas safe si son malloc fail
*/

char	*get_arg(char **str, char **envp, t_stdout **first_redi, char ***hrdc)
{
	unsigned int	i;
	char			quote;
	char			*arg;

	i = 0;
	while (*str && ft_isspace((*str)[i]))
		i++;
	if (!(*str) || (*str)[i] == '\0')
		return (NULL);
	quote = ft_strchr("'\"", (*str)[i]) ? (*str)[i] : (char)' ';
	arg = ft_strsub(*str, i, len_arg(*str + i, quote));
//	shell_envpsub(&arg, envp, quote); <-- plus ici, on le fait dans process
	shell_redi(&arg, first_redi, quote);
	*hrdc = shell_heredoc(&arg, quote, *hrdc);
	*str = *str + i + len_arg(*str + i, quote);
	return (arg);
}

t_cmd 	*get_args(char **line, char **envp, e_prompt *prompt)
{
	int		i;
	int 	nb_arg;
	t_cmd	*cmd;

	nb_arg = 0;
	if (!(*line) || (*line)[0] == '\0' || !(nb_arg = get_nbarg(*line, prompt)))
		return (NULL);
	cmd = (t_cmd *)shl_mlc("cmd", 3, &line, envp, sizeof(t_cmd));
	cmd->args = (char **)malloc(sizeof(char *) * (nb_arg + 1));
	cmd->args[nb_arg] = NULL;
	cmd->std_out = NULL;
	cmd->hrdc = NULL;
	i = 0;
	while (i < nb_arg)
		cmd->args[i++] = get_arg(line, envp, &cmd->std_out, &cmd->hrdc);
	//erase or create files for redi
	return (cmd);
}
