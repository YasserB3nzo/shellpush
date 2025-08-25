/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 01:05:55 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	token1(t_cmds *cmds, char c)
{
	if (c == '<')
	{
		if (cmds->prev && cmds->prev->token == Non)
			cmds->prev->token = Cmd;
		if (cmds->next && cmds->next->token == Non)
			cmds->next->token = Infile;
		cmds->token = Input;
	}
	else if (c == '>')
	{
		if (cmds->prev && cmds->prev->token == Non)
			cmds->prev->token = Cmd;
		if (cmds->next && cmds->next->token == Non)
			cmds->next->token = OutFile;
		cmds->token = Output;
	}
	else if (c == '|')
	{
		if (cmds->prev && cmds->prev->token == Non)
			cmds->prev->token = Cmd;
		if (cmds->next && cmds->next->token == Non)
			cmds->next->token = Cmd;
		cmds->token = Pipe;
	}
}

void	token2(t_cmds *cmds, int token_type)
{
	if (token_type == 1)
	{
		if (cmds->prev && cmds->prev->token == Non)
			cmds->prev->token = Cmd;
		if (cmds->next && cmds->next->token == Non)
			cmds->next->token = AppendFile;
		cmds->token = Append;
	}
	else if (token_type == 2)
	{
		if (cmds->prev && cmds->prev->token == Non)
			cmds->prev->token = Cmd;
		if (cmds->next && cmds->next->token == Non)
			cmds->next->token = HereDocDel;
		cmds->token = HereDoc;
	}
}

int	check_all_lt(t_cmds *cmds, int token_size)
{
	int	all_lt;
	int	i;

	all_lt = 1;
	i = 0;
	while (i < token_size && all_lt)
	{
		if (cmds->cmd[0][i] != '<')
			all_lt = 0;
		i++;
	}
	if (all_lt)
		return (print_all_lt_error(token_size));
	return (0);
}

void	assign_simple_tokens(t_cmds *cmds, int token_size)
{
	if (token_size == 1 && cmds->cmd[0][0] == '<')
		token1(cmds, '<');
	else if (token_size == 1 && cmds->cmd[0][0] == '>')
		token1(cmds, '>');
	else if (token_size == 1 && cmds->cmd[0][0] == '|')
		token1(cmds, '|');
	else if (token_size == 2 && cmds->cmd[0][0] == '>'
		&& cmds->cmd[0][1] == '>')
		token2(cmds, 1);
	else if (token_size == 2 && cmds->cmd[0][0] == '<'
		&& cmds->cmd[0][1] == '<')
		token2(cmds, 2);
}

void	assign_cmd_token(t_cmds *cmds)
{
	if (!cmds->prev && !cmds->next)
		cmds->token = Cmd;
	else if (cmds->token == Non && cmds->prev && (cmds->prev->token == OutFile
			|| cmds->prev->token == Infile || cmds->prev->token == Append
			|| cmds->prev->token == HereDocDel))
		cmds->token = Cmd;
}
