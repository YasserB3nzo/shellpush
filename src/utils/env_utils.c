/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:58:26 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/23 18:26:57 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*remove_node(t_env *head, t_env *node_to_remove)
{
	if (!head || !node_to_remove)
		return (head);
	if (node_to_remove == head)
	{
		head = node_to_remove->next;
		if (head)
			head->prev = NULL;
		free(node_to_remove->var_name);
		free(node_to_remove);
		return (head);
	}
	if (node_to_remove->prev)
		node_to_remove->prev->next = node_to_remove->next;
	if (node_to_remove->next)
		node_to_remove->next->prev = node_to_remove->prev;
	free(node_to_remove->var_name);
	free(node_to_remove);
	return (head);
}

t_env	*unset_env(t_env *list, char **command_args, t_data *data)
{
	t_env	*env_node;
	int		i;

	i = 0;
	env_node = NULL;
	while (command_args[i])
	{
		if (is_it_inside(command_args[i]) == false)
		{
			if (data->path_flag == true && ft_strcmp(command_args[i],
					"PATH") == 0)
				data->path_flag = false;
			env_node = find_env_variable(list, command_args[i]);
			if (env_node)
				list = remove_node(list, env_node);
		}
		i++;
	}
	return (list);
}

int	is_numeric(char *str)
{
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (1);
		str++;
	}
	return (0);
}
