/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util17.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 02:28:41 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 23:06:43 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_spaces(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

void	non_token(t_cmds *lst)
{
	while (lst)
	{
		if (lst->token == Non)
			lst->token = Cmd;
		lst = lst->next;
	}
}

static void	print_three(void)
{
	ft_putstr_fd("minishell: syntax error near ", 2);
	ft_putstr_fd("unexpected token `newline'\n", 2);
}

static int	print_seq_error(int count)
{
	if (count == 3)
		print_three();
	else if (count == 4)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putstr_fd("`<'\n", 2);
	}
	else if (count == 5)
	{
		ft_putstr_fd("minishell: syntax error near ", 2);
		ft_putstr_fd("unexpected token `<<'\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putstr_fd("`<<<'\n", 2);
	}
	return (2);
}

int	check_invalid_lt_sequences(char *line)
{
	int		count;
	int		i;
	char	c;

	i = 0;
	while (line && line[i])
	{
		if (line[i] == '<' || line[i] == '>')
		{
			c = line[i];
			count = 0;
			while (line[i + count] && line[i + count] == c)
				count++;
			if (count > 2)
				return (print_seq_error(count));
			i += count;
		}
		else
			i++;
	}
	return (0);
}
