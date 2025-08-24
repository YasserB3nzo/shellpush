/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:41:35 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/22 15:07:17 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	{
		printf("minishell: ");
		printf(command_args[1], 2);
		printf(": numeric argument required\n");
		exit(2);
	}
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

char	*get_my_path(t_env *list, char **command_args, bool flag, int i)
{
	char	**path_array;
	char	*path_value;
	char	*search_path;
	char	*command_path;

	search_path = NULL;
	if (command_args[0][0] == '/' || command_args[0][0] == '.')
		return (ft_strdup(command_args[0]));
	path_value = find_variable_value(list, list, "PATH", flag);
	if (!path_value)
		return (NULL);
	path_array = ft_split(path_value, ':');
	if (!path_array)
		return (NULL);
	while (path_array[i])
	{
		command_path = ft_strjoin3(path_array[i], '/', command_args[0]);
		if (!command_path)
		{
			free_array(path_array);
			return (NULL);
		}
		if (access(command_path, X_OK) == 0)
		{
			search_path = command_path;
			break ;
		}
		free(command_path);
		i++;
	}
	free_array(path_array);
	return (search_path);
}

char	**get_vars(char *cmd)
{
	char	**var;
	int		k;
	int		j;

	var = malloc(sizeof(char *) * how_many_dollar_in(cmd));
	k = 0;
	j = 0;
	while (cmd[j])
	{
		if (cmd[j] == 39 && check_ex(cmd, j) == true)
		{
			j++;
			while (cmd[j] && cmd[j] != 39)
				j++;
		}
		if (cmd[j] == '$' && cmd[j + 1] && cmd[j + 1] != '$')
			var[k++] = extract_variable_name(cmd + j, 0, 0, 0);
		else if (cmd[j] == '$' && cmd[j + 1] == '$')
			j++;
		if (cmd[j])
			j++;
	}
	var[k] = NULL;
	return (var);
}

char	**get_key_and_value(char *str, char **ptr, int i, int j)
{
	while (str[i])
	{
		if (str[i] == '=')
			j++;
		i++;
	}
	if (j == 0)
		return (get_name(str));
	else
		ptr = malloc(sizeof(char *) * 3);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	ptr[0] = malloc(sizeof(char) * (i + 1));
	j = -1;
	while (++j < i)
		ptr[0][j] = str[j];
	ptr[0][j] = '\0';
	i++;
	ptr[1] = ft_strdup(str + i);
	ptr[2] = NULL;
	return (ptr);
}
