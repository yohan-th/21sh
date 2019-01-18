/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   tabulator_check_if_var.c                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/01/08 17:22:56 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/16 22:30:24 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "shell.h"

static char		*get_var_name(char *env_var)
{
	int i;

	i = -1;
	while (env_var && env_var[++i])
		if (env_var[i] == '=')
			return (ft_strsub(env_var, 0, i));
	return (NULL);
}

static char		*get_path_var(char *name)
{
	int		first_char;
	char	*var;

	first_char = name[1] == '{' ? 2 : 1;
	var = NULL;
	if (ft_strlen(name + first_char))
		var = ft_strdup(name + first_char);
	return (var);
}

static char		*tabulator_var_full_name(char *name, char *var)
{
	char *full_var;

	full_var = NULL;
	if (name && name[1] == '{')
	{
		full_var = ft_strjoin("${", var);
		ft_strjoin_free(&full_var, "}");
	}
	else
		full_var = ft_strjoin("$", var);
	return (full_var);
}

void			tabulator_check_if_var_fill_list(t_tab *tb,
				t_tab_elem **list, char *var)
{
	t_tab_elem *new;

	if (!(new = malloc(sizeof(t_tab_elem))))
		return ;
	new->d_name = tabulator_var_full_name(tb->path, var);
	if (!tb->comp || ft_strlen(tb->comp))
		tabulator_autocomplete(&tb->comp, var);
	new->path = NULL;
	new->st_mode = 0;
	new->d_namlen = ft_strlen(new->d_name);
	tb->max_len = new->d_namlen > tb->max_len ? new->d_namlen : tb->max_len;
	new->d_type = 8;
	new->next = NULL;
	new->prev = NULL;
	if (*list)
	{
		(*list)->next = new;
		new->prev = *list;
	}
	*list = new;
	tb->elem = !tb->elem ? new : tb->elem;
	tb->nb_node++;
}

int				tabulator_check_if_var(t_tab *tabu)
{
	char		*var;
	char		*env_var_name;
	int			i;
	t_tab_elem	*list;

	var = get_path_var(tabu->path);
	env_var_name = NULL;
	i = 0;
	list = NULL;
	while (tabu && tabu->env[i])
	{
		env_var_name = get_var_name(tabu->env[i]);
		if (!var || (env_var_name &&
		!ft_strncmp(env_var_name, var, ft_strlen(var))))
			tabulator_check_if_var_fill_list(tabu, &list, env_var_name);
		ft_strdel(&env_var_name);
		i++;
	}
	ft_strdel(&var);
	return (0);
}
