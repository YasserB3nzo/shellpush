/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syserr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:13:59 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 18:32:03 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Consolidated error message function */
int	print_syntax_error(char *token, bool duplicate)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(token, 2);
	if (duplicate)
		ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	return (2);
}

int	errormsg_v1(char *str)
{
	return (print_syntax_error(str, false));
}

int	errormsg_v2(char *str)
{
	return (print_syntax_error(str, true));
}

int	check_for_pipe(t_cmds *cmds)
{
	if (!cmds->prev || cmds->prev->token == Non)
	{
		if (cmds->next && cmds->next->token == Pipe)
			return (errormsg(" `||'\n"));
		return (errormsg(" `|'\n"));
	}
	else if (cmds->prev && cmds->prev->token != Pipe)
	{
		if (cmds->next && cmds->next->token == Pipe)
		{
			if (cmds->next->token == Pipe)
				return (errormsg(" `||'\n"));
			else if (cmds->next->next && cmds->next->next->token == Pipe)
			{
				if (cmds->next->next->next
					&& cmds->next->next->next->token == Pipe)
					return (errormsg(" `||'\n"));
				return (errormsg(" `|'\n"));
			}
		}
		else if (!cmds->next || (cmds->next && cmds->next->token == Non
				&& !cmds->next->next))
			return (errormsg(" 'newline'\n"));
	}
	return (0);
}

int	check_quotation(char *str)
{
	char	c;
	int		i;

	i = -1;
	if (!str)
		return (-1);
	while (str[++i])
	{
		if (str[i] == 39 || str[i] == 34)
		{
			c = str[i];
			while (str[++i])
			{
				if (str[i] == c)
					break ;
			}
		}
		if (str[i] == '\0')
		{
			errormsg(" 'newline'\n");
			return (-1);
		}
	}
	return (0);
}
