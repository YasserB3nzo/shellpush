/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:41:35 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	exit_numeric_error(char *arg)
{
	printf("minishell: ");
	printf(arg, 2);
	printf(": numeric argument required\n");
	exit(2);
}

void	exit_from_child(char **command_args)
{
	int	i;

	if (command_args[1] == NULL)
		exit(g_signal.ret_exit);
	else if (is_numeric(command_args[1]) != 0)
		exit(2);
	else if (command_args[1] && command_args[2])
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	else
	{
		i = ft_atoi(command_args[1]);
		g_signal.ret = i;
		exit(i);
	}
}

void	exit_myminishell(char **command_args, int flag)
{
	int	i;

	if (flag != 0)
		exit_from_child(command_args);
	else if (command_args[1] == NULL)
	{
		printf("exit\n");
		exit(g_signal.ret_exit);
	}
	else if (is_numeric(command_args[1]) != 0)
		exit_numeric_error(command_args[1]);
	else if (command_args[1] && command_args[2])
		printf("minishell: exit: too many arguments\n");
	else
	{
		i = ft_atoi(command_args[1]);
		printf("exit\n");
		g_signal.ret = i;
		exit(i);
	}
}
