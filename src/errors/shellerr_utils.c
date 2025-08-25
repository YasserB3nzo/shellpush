/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellerr_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:41:02 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	loop_tokens(t_cmds *curr, int i)
{
	while (curr && i == 0)
	{
		if (curr->token == Pipe)
			i = check_for_pipe(curr);
		else if (curr->token == Output || curr->token == Input)
			i = check_for_in_out_put(curr);
		else if (curr->token == Append || curr->token == HereDoc)
			i = check_for_append_heredoc(curr);
		curr = curr->next;
	}
	return (i);
}

int	process_tokens_loop(t_cmds *curr, int i)
{
	return (loop_tokens(curr, i));
}

int	errors_managment(t_data *data, int i)
{
	t_cmds	*curr;
	t_cmds	*head;

	curr = data->lst;
	head = curr;
	i = heredoc(head, head, i, 0);
	if (i != 0)
		return (i);
	return (process_tokens_loop(curr, i));
}
