/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:10:06 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/20 14:10:10 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exiterror(void)
{
	write(1, "error\n", 7);
	return ;
}

bool	check_n_flag(char *str)
{
	str++;
	while (str && *str)
	{
		if (*str != 'n')
			return (false);
		str++;
	}
	return (true);
}

void	ft_echo(char **command_args, bool flag, int i)
{
	if (!command_args[0])
		ft_putstr_fd("\n", 1);
	if (!command_args[0])
		return ;
	if (command_args[0][0] == '-' && command_args[0][1] == 'n')
	{
		while (command_args[0] && command_args[0][0] == '-')
		{
			if (check_n_flag(command_args[0]) == true)
			{
				flag = false;
				command_args++;
			}
			else
				break ;
		}
	}
	while (command_args[i])
	{
		ft_putstr_fd(command_args[i++], 1);
		if (command_args[i])
			ft_putchar_fd(' ', 1);
	}
	if (flag == true)
		ft_putstr_fd("\n", 1);
}

char	**create_environment(void)
{
	char	**env_array;
	char	buffer[PATH_MAX];

	env_array = (char **)malloc((4) * sizeof(char *));
	if (!env_array)
		exiterror();
	env_array[0] = ft_strjoin("PWD=", getcwd(buffer, PATH_MAX));
	env_array[1] = ft_strdup("SHLVL=1");
	env_array[2] = ft_strdup("_=/usr/bin/env");
	env_array[3] = NULL;
	return (env_array);
}

t_env	*env_last(t_env *lst)
{
	if (lst == NULL)
		return (lst);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}
