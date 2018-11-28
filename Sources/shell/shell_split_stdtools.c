/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_split_stdtools.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/22 00:38:31 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/22 00:38:31 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../Include/shell.h"

int		len_stdout_to(char *str, char quote)
{
	int i;

	i = (quote == ' ') ? 0 : 1;
	while (str && str[i])
	{
		//printf("str[%d]<%c>\n", i, str[i]);
		if (str[i] == '\\' && ft_strlen(str) >= (i + 2) && quote != '\'')
			i += 2;
		if (ft_strchr("'\"", str[i]) && quote == ' ')
			quote = str[i];
		else if (str[i] == quote && quote != ' ')
			quote = ' ';
		if (quote == ' ' && ft_strchr("><", str[i]))
			break ;
		if (str[i] == quote && (quote == ' ' || ft_strchr("\0 ", str[i + 1])))
			break ;
		i += (str[i]) ? 1 : 0;
	}
	return (i);
}

t_stdout		*get_last_stdout(t_stdout *redi)
{
	t_stdout	*t_next;

	t_next = redi;
	while (t_next->next)
		t_next = t_next->next;
	return (t_next);
}

char		*complete_stdout_to(char **arg, t_stdout *add_to, char quote)
{
	char *ret;

	add_to->to = ft_strsub(*arg, 0, (size_t) len_stdout_to(*arg, quote));
	if ((ft_strlen(*arg) - len_stdout_to(*arg, quote)) > 0)
		ret = ft_strsub(*arg, (unsigned) len_stdout_to(*arg, quote),
						(size_t )ft_strlen(*arg));
	else
		ret = NULL;
	ft_strdel(arg);
	*arg = NULL;
	return (ret);
}

/*
** On réutilise les fonctions de heredoc car c'est le même principe.
** shell_std_in peut renvoyer NULL si malloc fail.
** {std_in} peut valoir soit une chaine de char, soit -1 si erreur, soit -2 si
** la chaine de char se trouve dans le prochain arg.
*/

char	**shell_std_in(char **arg, char quote, char **std_in)
{
	int		i;

	complete_hrdc(arg, quote, &std_in);
	i = (quote == ' ') ? 0 : 1;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) > (i + 2) && quote == ' ')
			i += 2;
		if (ft_strchr("'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '<' && quote == ' ')
		{
			if (!(std_in = add_hrdc(std_in)))
				return (NULL);
			(*arg)[i] = '\0';
			i = shell_hrdc_sub(arg, i + 1, &std_in);
		}
		else
			i++;
	}
	return (std_in);
}
