/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util10.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:15:41 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmds	*copy_node(char **cmd, t_token token, bool flag)
{
	t_cmds	*new_node;

	new_node = (t_cmds *)malloc(sizeof(t_cmds));
	if (flag == true)
		new_node->cmd = cmd;
	else
		new_node->cmd = get_name(cmd[0]);
	new_node->token = token;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_cmds	*copy_single_node(t_cmds *curr, int *i)
{
	if (curr->token == Cmd && *i == 0)
	{
		*i = 1;
		return (NULL);
	}
	return (copy_node(curr->cmd, curr->token, false));
}

static void	append_new_node(t_cmds *curr, int *i, t_cmds **new_head,
		t_cmds **new_tail)
{
	t_cmds	*new_node;

	new_node = copy_single_node(curr, i);
	if (!new_node)
		return ;
	if (!*new_head)
		*new_head = new_node;
	else
	{
		(*new_tail)->next = new_node;
		new_node->prev = *new_tail;
	}
	*new_tail = new_node;
}

static void	attach_list_head(t_cmds *head, t_cmds *new_head)
{
	if (new_head)
	{
		head->next = new_head;
		new_head->prev = head;
	}
}

t_cmds	*copy_list(t_cmds *curr, char **command, int i)
{
	t_cmds	*head;
	t_cmds	*new_head;
	t_cmds	*new_tail;

	new_tail = NULL;
	new_head = NULL;
	head = copy_node(command, Cmd, true);
	while (curr && curr->token != Pipe)
	{
		if (curr->token != Cmd || i == 0)
			append_new_node(curr, &i, &new_head, &new_tail);
		curr = curr->next;
	}
	attach_list_head(head, new_head);
	return (head);
}
