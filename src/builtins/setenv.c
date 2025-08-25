/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:16:52 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 21:16:55 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setenv_builtin(t_env *list, char **args)
{
	char	*var_string;
	char	**export_args;

	if (!args[1])
	{
		ft_putstr_fd("setenv: Too few arguments\n", 2);
		return ;
	}
	if (args[2])
		var_string = ft_strjoin3(args[1], '=', args[2]);
	else if (ft_strchr(args[1], '='))
		var_string = ft_strdup(args[1]);
	else
		var_string = ft_strjoin(args[1], "=");
	export_args = malloc(sizeof(char *) * 3);
	export_args[0] = ft_strdup("export");
	export_args[1] = var_string;
	export_args[2] = NULL;
	export(list, export_args, '-', 1);
	free_array(export_args);
}

void	unsetenv_builtin(t_env *list, char **args)
{
	char	**unset_args;
	int		i;
	int		arg_count;

	if (!args[1])
	{
		ft_putstr_fd("unsetenv: Too few arguments\n", 2);
		return ;
	}
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	unset_args = malloc(sizeof(char *) * (arg_count + 1));
	unset_args[0] = ft_strdup("unset");
	i = 1;
	while (args[i])
	{
		unset_args[i] = ft_strdup(args[i]);
		i++;
	}
	unset_args[i] = NULL;
	unset(list, unset_args, 1);
	free_array(unset_args);
}
