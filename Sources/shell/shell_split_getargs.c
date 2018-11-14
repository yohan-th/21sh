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

int 	get_nb_args(char *str, e_prompt *prompt)
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
** Delete les double \\ ainsi que les quotes inutiles --> tes"t $USER te"st
** Retourne 0 si les quotes ne sont pas fermé
*/

int 	shl_clean_arg(char *arg, char quote)
{
	arg += (quote == ' ') ? 0 : 1;
	while (*arg)
	{
		if (*arg == '\\' && quote != '\'')
		{
			ft_strdelchar(&arg, *arg);
			arg++;
		}
		else if (ft_strchr("'\"", *arg) && quote == ' ')
			ft_strdelchar(&arg, quote = *arg);
		else if (*arg == quote && quote != ' ' && *(arg + 1) != '\0')
		{
			ft_strdelchar(&arg, quote);
			quote = ' ';
		}
		else if (quote != ' ' && *arg == quote && *(arg + 1) == '\0')
			return (1);
		else
			arg++;
	}
	return ((quote != ' ') ? 0 : 1);
}

/*
** subtitue le premier {args_tab} de {str} et avance de {len} de {args_tab}.
** {arg} garde les quotes pour {shell_envsub} au moment du [process].
** /!\ shell_redi n'est pas safe si son malloc fail
*/

char	*get_arg(char **str, char **envp, t_redi **first_redi)
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
	*str = *str + i + len_arg(*str + i, quote);
	return (arg);
}

t_cmd 	*get_args(char **line, char **envp, e_prompt *prompt)
{
	int		i;
	int 	nb_args;
	t_cmd	*cmd;

	if (!(*line) || (*line)[0] == '\0' ||
			!(nb_args = get_nb_args(*line, prompt)))
	{
		if (!nb_args)
			dprintf(2, "exit par nb_args\n");
		return (NULL);
	}
	cmd = (t_cmd *)shl_mlc("cmd", 3, &line, envp, sizeof(t_cmd));
	cmd->args = (char **)malloc(sizeof(char *) * (nb_args + 1));
	cmd->args[nb_args] = NULL;
	cmd->redi = NULL;

	i = 0;
	while (i < nb_args)
		cmd->args[i++] = get_arg(line, envp, &cmd->redi);
	//erase or create files for redi
	return (cmd);
}
