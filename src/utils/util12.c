/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util12.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:07:26 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// isolated from util10.c to reduce function count

t_cmds	*merge_lists(t_cmds *list1, t_cmds *list2)
{
	t_cmds	*current;
	t_cmds	*pipe;

	if (!list1)
		return (list2);
	if (!list2)
		return (list1);
	pipe = copy_node(get_name("|"), Pipe, true);
	current = list1;
	while (current->next)
		current = current->next;
	current->next = pipe;
	pipe->prev = current;
	pipe->next = list2;
	list2->prev = pipe;
	return (list1);
}
