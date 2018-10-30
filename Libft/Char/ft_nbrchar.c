/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_nbrchar.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/03/14 17:21:11 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/03/14 17:21:11 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Includes/char.h"

int		ft_nbrchar(char *str, char c)
{
	int i;

	i = 0;
	while (*str)
	{
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}