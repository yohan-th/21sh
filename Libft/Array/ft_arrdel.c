/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_arrdel.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/18 20:00:38 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/06/18 20:00:39 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../Includes/array.h"

void	ft_arrdel(char ***arr)
{
	int i;

	i = 0;
	while (arr && *arr != NULL && (*arr)[i] != NULL)
		ft_strdel(&(*arr)[i++]);
	if (arr && *arr != NULL)
		free(*arr);
	arr = NULL;
}
