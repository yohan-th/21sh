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

#include "../../../Include/shell.h"

/*
** hrdc = NULL lors du premier heredoc
** le dernier ret[i] est à -1 pour indiquer qu'il n'est pas rempli
*/

char	**add_stdin(char **hrdc)
{
	char	**ret;
	int		i;

	i = 0;
	while (hrdc && hrdc[i] != NULL)
		i++;
	if (!(ret = (char **)malloc(sizeof(char *) * (i + 2))))
		return (NULL);
	i = 0;
	while (hrdc && hrdc[i] != NULL)
	{
		if ((int)hrdc[i] == -1 || (int)hrdc[i] == -2)
			ret[i] = (char *)hrdc[i];
		else
		{
			ret[i] = ft_strdup(hrdc[i]);
			ft_strdel(&hrdc[i]);
		}
		i++;
	}
	if (hrdc)
		free(hrdc);
	ret[i] = (char *)-1;
	ret[i + 1] = NULL;
	return (ret);
}

void	complete_stdin(char **arg, char quote, char ***std_in)
{
	int		last;
	int		lenhrdc;
	char	*tmp;

	last = 0;
	lenhrdc = 0;
	while (*std_in && (*std_in)[last] != NULL)
		last++;
	if (*std_in && (int)(*std_in)[last - 1] == -2 && len_stdin(*arg, quote) > 0)
	{
		lenhrdc = len_stdin(*arg, quote);
		(*std_in)[last - 1] = ft_strsub(*arg, (unsigned)0, (size_t)lenhrdc);
		**arg = '\0';
	}
	tmp = *arg;
	*arg = ft_strdup(*arg + lenhrdc);
	ft_strdel(&tmp);
}

/*
** stdin_sub renvoi la valeur de combien stdin a été sub, (idealement reçu par
** {i})
*/

int			shell_get_stdin(char ***ptn_stdin, char **arg, int i)
{
	char *tmp;

	*ptn_stdin = add_stdin(*ptn_stdin);
	i = shell_stdin_sub(arg, i + 1, ptn_stdin);
	tmp = *arg;
	*arg = ft_strdup(*arg + i);
	ft_strdel(&tmp);
	return (0);
}

int 		shell_get_hrdc(char ***ptn_hrdc, char **arg, int i,
							char ***ptn_stdin, char **hrdc_stdin)
{
	char	*tmp;
	int		last;

	*ptn_hrdc = add_stdin(*ptn_hrdc);
	i = shell_stdin_sub(arg, i + 2, ptn_hrdc);
	tmp = *arg;
	*arg = ft_strdup(*arg + i);
	ft_strdel(&tmp);
	*ptn_stdin = add_stdin(*ptn_stdin);
	last = 0;
	while ((*ptn_stdin)[last] != NULL)
		last++;
	(*ptn_stdin)[last - 1] = *hrdc_stdin;
	return (0);
}

/*
** {hrdc[i] et std_in[i]} peuvent valoir soit une string, soit -1 si non rempli
** soit -2 si la chaine de char se trouve dans le prochain arg.
** Dans shell_hrdc, {hrdc[i]} prendra la valeur de -3 pour être indiqué
** comme rempli par l'utilisateur.
** hrdc_stdin (dont son adresse sera rentré dans **stdin) prend la valeur -1
** pour ne pas stopper la lecture de stdin et prendra la valeur rentré par user
*/

void	shell_std_in(char **arg, char quote, char ***ptn_stdin,
					char ***ptn_hrdc, char **hrdc_stdin)
{
	int		i;

	complete_stdin(arg, quote, ptn_hrdc);
	complete_stdin(arg, quote, ptn_stdin);
	i = (quote == ' ') ? 0 : 1;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '\\' && ft_strlen(*arg) >= (i + 2) && quote != '\'')
			i += 2;
		if (ft_strchr("'\"", (*arg)[i]) && quote == ' ')
			quote = (*arg)[i];
		else if ((*arg)[i] == quote && quote != ' ')
			quote = ' ';
		if ((*arg)[i] == '<' && (*arg)[i + 1] == '<' && quote == ' ')
		{
			*hrdc_stdin = (char *)-1;
			i = shell_get_hrdc(ptn_hrdc, arg, i, ptn_stdin, hrdc_stdin);
		}
		else if ((*arg)[i] == '<' && quote == ' ')
			i = shell_get_stdin(ptn_stdin, arg, i);
		else
			i += ((*arg)[i]) ? 1 : 0;
	}
}

/*
** Note de fin :
** L'inconvéniant d'attribuer des valeurs {-1}, {-2} et {-3} à un char* est
** qu'il nécessite un free particulier car valeurs numériques non malloc
*/
