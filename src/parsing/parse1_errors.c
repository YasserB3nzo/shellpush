/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 01:06:04 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/25 01:02:50 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	print_all_lt_error_internal(int token_size)
{
	if (token_size == 3)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token", 2);
		ft_putstr_fd(" `newline'\n", 2);
	}
	else if (token_size == 4)
		ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
	else if (token_size == 5)
		ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
	else
		ft_putstr_fd("minishell: syntax error near unexpected token `<<<'\n",
			2);
	return (2);
}

int	print_all_lt_error(int token_size)
{
	return (print_all_lt_error_internal(token_size));
}
