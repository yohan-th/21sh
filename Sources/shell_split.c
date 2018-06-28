/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   split_mnshl.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/15 18:53:39 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/05/15 18:53:39 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Include/shell.h"

int 	check_last_quote(char *arg, char quote)
{
	if (quote != ' ' && *arg != quote)
		return (0);
	else if (quote != ' ' && *arg == quote)
		ft_strdelchar(&arg, quote);
	return (1);
}

int		mnshl_quotesub(char *arg)
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

void	mnshl_envpsub(char **arg, char **envp)
{
	char	quote;
	int		i;

	i = 0;
	quote = ft_strchr("'\"`", (*arg)[i]) ? (*arg)[i] : (char)' ';
	while ((*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2))
			i += 2;
		if (ft_strchr("`'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '$' && quote != '\'')
			i = shell_argsub_env(arg, i, envp);
		i++;
	}
}

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
		if (str[i - 1] != '\\' && quote == ' ' &&
			(str[i] == ';' || str[i] == '&' ||
			(str[i] == '|' && str[i + 1] == '|')))
			break ;
		if (str[i] == quote && (quote == ' ' || ft_strchr("\0 ", str[i + 1])))
			break ;
	}
	return (i);
}

//echo ~ ~te~st" ~ '$USER  \""+\\$USER+$US\ER~$USERS' ~ t"e$USER \'~'' ""'`' ""' \' ""'" \'>


char	*get_arg(char **str, char **envp)
{
	unsigned int	i;
	char			quote;
	char 			*arg;

	i = 0;
	while (*str && ft_strchr("\t ", (*str)[i]))
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
	mnshl_envpsub(&arg, envp);
	if (!(mnshl_quotesub(arg)))
		ft_strdel(&arg);
	else
		*str = *str + i + len_arg(*str + i, quote);
	return (arg);
}

/*
** Est-ce qu'un fail de malloc doit quitter le programme ou retenter un malloc ?
*/

t_cmd	*shell_split(char *line, char **envp)
{
	t_cmd	*cmd;
	t_cmd	*prev;
	char 	*arg;

	prev = NULL;
	cmd = ft_memalloc(sizeof(t_cmd));
	while ((arg = get_arg(&line, envp)))
	{
		cmd->arg = arg;
		cmd->start = (prev) ? prev->start : cmd;
		prev = cmd;
		cmd->next_cmd = ft_memalloc(sizeof(t_cmd));
		cmd = cmd->next_cmd;
	}
	if (ft_strlen(line) > 0)
	{
		clean_cmd(&cmd);
		return (NULL);
	}
	else
		return ((prev) ? prev->start : NULL);
}
