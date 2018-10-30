/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strjoin_free.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/01 23:46:11 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/30 19:34:59 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char *tmp;

	if (!s1)
		tmp = ft_strdup(s2);
	else
	{
		tmp = ft_strjoin(s1, s2);
		ft_strdel(&s1);
	}
	return (tmp);
}
