/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:38:01 by ksohail-          #+#    #+#             */
/*   Updated: 2025/08/20 18:14:30 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**sort_export_array(char **arr, int n)
{
	int	i;
	int	j;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
				swap(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
	return (arr);
}

int	print_export_variables(t_env *list)
{
	int		i;
	char	**env_array;

	i = 0;
	env_array = linked_list_to_array(list);
	while (env_array[i])
		i++;
	sort_export_array(env_array, i);
	i = 0;
	while (env_array[i])
	{
		if (env_array[i][0] == '_' && env_array[i][1] == '=')
			i++;
		else
		{
			printf("declare -x ");
			print_value(env_array[i]);
			i++;
		}
	}
	free_array(env_array);
	return (0);
}

int	ft_all_isalpha(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalpha(str[i]) || str[i] == '_' || str[i] == '+'
			|| str[i] == '\t'))
	{
		i++;
	}
	if ((str[i] == '=' && i != 0) || str[i] == '\0')
		return (0);
	return (1);
}

char	*fill_var(char *var, char *c)
{
	char	*str;
	int		j;

	*c = '+';
	str = malloc(sizeof(char) * ft_strlen(var));
	j = -1;
	while (++j >= 0 && var[j] != '+')
		str[j] = var[j];
	str[j] = '\0';
	free(var);
	return (str);
}

void	export(t_env *list, char **command_args, char c, int i)
{
	char	**export;
	bool	export_flag;

	if (command_args[1] == NULL && print_export_variables(list) == 0)
		return ;
	while (command_args[i])
	{
		c = '-';
		export_flag = false;
		if (ft_all_isalpha(command_args[i]) == 1)
			printf("export: '%s' :not a valid identifier\n", command_args[i]);
		else
		{
			if (command_args[i][ft_strlen(command_args[i]) - 1] == '=')
				export_flag = true;
			export = get_key_and_value(command_args[i], NULL, 0, 0);
			if (export[0][ft_strlen(export[0]) - 1] == '+')
				export[0] = fill_var(export[0], &c);
			set_env_after_export(list, export, c, export_flag);
			free_array(export);
		}
		i++;
	}
}
