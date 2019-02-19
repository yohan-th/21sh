/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_arrcpy.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/19 11:51:13 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/19 14:59:32 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "array.h"

char	**ft_arrcpy(char **arr_dst, char **arr_src)
{
	int i;

	i = 0;
	while (arr_dst && arr_src && arr_src[i])
	{
		arr_dst[i] = arr_src[i];
		i++;
	}
	return (arr_dst);
}
