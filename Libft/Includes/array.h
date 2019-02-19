/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   array.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/18 19:57:11 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/19 12:17:34 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef ARRAY_H

# define ARRAY_H

# include "libft.h"

char	**ft_arrdup(char **arr);
int		ft_arrlen(char **arr);
void	ft_arrdel(char ***arr);
char	**ft_arrnew(size_t size);
char	**ft_arrcpy(char **arr_dst, char **arr_src);

#endif
