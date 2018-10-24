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

/*
** Calcul la longueur de l'argument
** Les conditions de fin sont :
**  - match de nouveau le charactere de {quote}
**  - match avec {;} OU {&&} OU {|}
*/

size_t	len_arg(char *str, char quote)
{
	size_t i;

	i = 0;
	while (*str && str[i])
	{
		i++;
		if (str[i - 1] == '\\' && str[i] == quote && quote != '\'')
			i++;
		if (ft_strchr("'\"", str[i]) && quote == ' ')
			quote = str[i++];
		if (str[i] == quote && quote != ' ' && (!ft_strchr("\0 ", str[i + 1])))
			quote = ' ';
		if (quote == ' ' && str[i - 1] != '\\' && (ft_strchr(";|", str[i]) ||
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
	int 	nb_args;

	nb_args = 0;
	while (*str && !ft_strchr(";|", str[0]) &&
			(str[0] != '&' && str[1] != '&'))
	{
		str = shell_trim(&str);
		quote = ft_strchr("'\"", *str) ? *str : (char)' ';
		str += len_arg(str, quote);
		nb_args += 1;
		str = shell_trim(&str);
	}
	printf("nb arg %d\n", nb_args);
	return (nb_args);
}

/*
** Delete les double \\ ainsi que les quotes inutiles --> tes"t $USER te"st
** Retourne 0 si les quotes ne sont pas fermé
*/

int 	shl_clean_arg(char *arg, char quote)
{
	if (quote != ' ')
		arg++;
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
		else if (*arg == quote && quote != ' ' && *(arg + 1) == '\0')
			return (1);
		else
			arg++;
	}
	return ((quote != ' ') ? 0 : 1);
}

/*
** subtitue le premier args_tab de str et avance str de len de args_tab
** On profite d'avoir les quotes pour remplacer les var env et redi
** On laisse les backquotes pour les executer après
** /!\ shell_redi n'est pas safe sur son malloc fail
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
	shell_envpsub(&arg, envp, quote);
	shell_redi(&arg, first_redi, quote);
	if (!shl_clean_arg(arg, quote))
		ft_strdel(&arg);
	else
		*str = *str + i + len_arg(*str + i, quote);
	return (arg);
}

t_cmd 	*get_args(char **line, char **envp)
{
	int		i;
	int 	nb_args;
	t_cmd	*cmd;

	if (!(*line) || (*line)[0] == '\0')
		return (NULL);
	cmd = (t_cmd *)shl_mlc("cmd", 3, &line, envp, sizeof(t_cmd));
	nb_args = get_nb_args(*line);
	cmd->args = (char **)malloc(sizeof(char *) * (nb_args + 1));
	cmd->args[nb_args] = NULL;
	cmd->redi = NULL;

	i = 0;
	while (i < nb_args)
		cmd->args[i++] = get_arg(line, envp, &cmd->redi);
	if (nb_args > 0 && cmd->args[nb_args - 1] == NULL) //ou parsing redi error
	{
		//clean_cmd(&cmd);
		//clean args avec args->args_tab
		ft_arrdel(cmd->args);
		return (NULL);
	}
	//prepare redi (erase or create files)
	return (cmd);
}
