/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:09:24 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/22 15:09:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	commands_clear(t_command **command_list)
{
	t_command	*current;
	t_command	*next_command;

	if (command_list == NULL || *command_list == NULL)
		return ;
	current = *command_list;
	while (current->next != NULL)
	{
		next_command = current->next;
		free_array(current->cmd);
		slist_clear(&current->infile);
		slist_clear(&current->outfile);
		free(current);
		current = next_command;
	}
	free_array(current->cmd);
	slist_clear(&current->infile);
	slist_clear(&current->outfile);
	free(current);
	*command_list = NULL;
}

void	lstclear(t_cmds **command_list)
{
	t_cmds	*current;
	t_cmds	*next_cmd;

	if (command_list == NULL || *command_list == NULL)
		return ;
	current = *command_list;
	while (current->next != NULL)
	{
		next_cmd = current->next;
		free_array(current->cmd);
		free(current);
		current = next_cmd;
	}
	free_array(current->cmd);
	free(current);
	*command_list = NULL;
}

t_cmds	*lstlast(t_cmds *command_list)
{
	if (command_list == NULL)
		return (command_list);
	while (command_list->next)
	{
		command_list = command_list->next;
	}
	return (command_list);
}

t_command	*command_last(t_command *command_list)
{
	if (command_list == NULL)
		return (command_list);
	while (command_list->next)
	{
		command_list = command_list->next;
	}
	return (command_list);
}

t_slist	*nodes_last(t_slist *node_list)
{
	if (node_list == NULL)
		return (node_list);
	while (node_list->next)
	{
		node_list = node_list->next;
	}
	return (node_list);
}
