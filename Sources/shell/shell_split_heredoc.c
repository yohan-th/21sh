/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split_heredoc.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/18 16:14:04 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/18 16:14:04 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int		len_hrdc(char *str, char quote)
{
	int i;

	i = (quote == ' ') ? 0 : 1;
	while (*str && str[i])
	{
		if (str[i] == '\\' && ft_strlen(str) > (i + 2) && quote == ' ')
			i += 2;
		if (ft_strchr("'\"", str[i]) && quote == ' ')
			quote = str[i];
		else if (str[i] == quote && quote != ' ')
			quote = ' ';
		if (quote == ' ' && ft_strchr("><", str[i]))
			break ;
		if (str[i] == quote && (quote == ' ' || ft_strchr("\0 ", str[i + 1])))
			break ;
		i++;
	}
	return (i);
}

int 	shell_hrdc_sub(char **arg, int i, char ***hrdc)
{
	int 	last;
	char 	quote;
	int 	len;

	last = 0;
	while ((*hrdc)[last] != NULL)
		last++;
	if ((*arg)[i] == '\0') //redi prochain arg
	{
		(*hrdc)[last - 1] = (char *)-2;
		return (i);
	}
	quote = ft_strchr("\"'", **arg) ? **arg : (char)' ';
	len = len_hrdc(*arg + i, quote);
	if (len > 0)
		(*hrdc)[last - 1] = ft_strsub(*arg, (unsigned)i, (size_t)len);
	return (i + len);
}

/*
** hrdc = NULL lors du premier heredoc
** le dernier ret[i] est à -1 pour indiquer qu'il n'est pas rempli
*/

char	**add_hrdc(char **hrdc)
{
	char 	**ret;
	int		i;

	if (hrdc == NULL)
	{
		if (!(ret = (char **)malloc(sizeof(char *) * 2)))
			return (NULL);
		ret[0] = (char *)-1;
		ret[1] = NULL;
		return (ret);
	}
	i = 0;
	while (hrdc[i] != NULL)
		i++;
	if (!(ret = (char **)malloc(sizeof(char *) * (i + 2))))
		return (NULL);
	i = 0;
	while (hrdc[i] != NULL)
	{
		if ((int)hrdc[i] == -1)
			ret[i++] = (char *)-1;
		else
		{
			ret[i] = ft_strdup(hrdc[i]);
			ft_strdel(&hrdc[i++]);
		}
	}
	free(hrdc);
	ret[i] = (char *)-1;
	ret[i + 1] = NULL;
	return (ret);
}

void		complete_hrdc(char **arg, char quote, char ***hrdc)
{
	int 	last;

	last = 0;
	while (*hrdc && (*hrdc)[last] != NULL)
		last++;
	if (*hrdc && (int)(*hrdc)[last - 1] == -2 && len_hrdc(*arg, quote) > 0)
	{
		(*hrdc)[last - 1] = ft_strsub(*arg, (unsigned)0, (size_t)len_hrdc(*arg, quote));
		**arg = '\0';
	}
	arg = arg + len_hrdc(*arg, quote);
}

/*
** shell redi peut renvoyer NULL si malloc fail.
** {hrdc} peut valoir soit une chaine de char, soit -1 si erreur, soit -2 si
** la chaine de char se trouve dans le prochain arg.
*/

char		**shell_heredoc(char **arg, char quote, char **hrdc)
{
	int		i;

	complete_hrdc(arg, quote, &hrdc);
	i = (quote == ' ') ? 0 : 1;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2) && quote == ' ')
			i += 2;
		if (ft_strchr("'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '<' && (*arg)[i + 1] == '<' && quote == ' ')
		{
			if (!(hrdc = add_hrdc(hrdc)))
				return (NULL);
			(*arg)[i] = '\0';
			//printf("<%s> et <%s>\n", hrdc[0], hrdc[1]);
			i = shell_hrdc_sub(arg, i + 2, &hrdc);
		}
		else
			i++;
	}
	//printf("hrdc[0]=<%d> hrdc[1]=<%s>\n", (int)hrdc[0], hrdc[1]);
	return (hrdc);
}

