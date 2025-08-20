/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:10:00 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/20 14:10:47 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_env_after_export(t_env *list, char **export, char c,
		bool export_flag)
{
	t_env	*env_node;

	env_node = find_env_variable(list, export[0]);
	if (env_node)
	{
		if (c == '+')
			set_env_if_plus(env_node, export[1]);
		else
		{
			if (export[1] != NULL || export_flag == true)
				free(env_node->var_name);
			if (export[1] != NULL)
				env_node->var_name = ft_strjoin3(export[0], '=', export[1]);
			else if (export_flag == true)
				env_node->var_name = ft_strjoin3(export[0], '=', "");
		}
		return ;
	}
	list = env_last(list);
	list->next = env_new(list, ft_strjoin3(export[0], '=', export[1]));
}

void	set_env_after_cd(t_env *list, char *key, char *value)
{
	t_env	*env_node;
	t_env	*node;
	char	*tmp;

	env_node = find_env_variable(list, key);
	if (env_node && (ft_strcmp(key, "OLDPWD") == 0 || ft_strcmp(key, "PWD") == 0))
	{
		free(env_node->var_name);
		env_node->var_name = ft_strjoin(key, "=");
		if (value)
		{
			tmp = env_node->var_name;
			env_node->var_name = ft_strjoin(env_node->var_name, value);
			free(tmp);
		}
	}
	else if (!env_node && (ft_strcmp(key, "PWD") == 0 || ft_strcmp(key,
				"OLDPWD") == 0))
	{
		node = env_new(list, ft_strjoin3(key, '=', value));
		list = env_last(list);
		list->next = node;
	}
}

void	change_directory_path(t_env *list, char *path)
{
	char	*current_pwd;
	char	buffer[PATH_MAX];

	current_pwd = find_variable_value(list, list, "PWD", false);
	if (path[0] == '\0')
		return ;
	if (chdir(path) != 0)
		perror("cd");
	else
	{
		set_env_after_cd(list, "OLDPWD", current_pwd);
		set_env_after_cd(list, "PWD", getcwd(buffer, PATH_MAX));
	}
}

char	*find_variable_value(t_env *list, t_env *head, char *variable_name, bool flag)
{
	char	**value_parts;

	while (list)
	{
		value_parts = ft_split(list->var_name, '=');
		if (ft_strcmp(value_parts[0], variable_name) == 0)
		{
			free_array(value_parts);
			return (ft_strchr(list->var_name, '=') + 1);
		}
		free_array(value_parts);
		if (!list->next)
			break ;
		list = list->next;
	}
	while (head)
	{
		if (ft_strncmp(head->var_name, "path", 3) == 0)
			return (NULL);
		head = head->next;
	}
	if (ft_strncmp(variable_name, "PATH", 3) == 0 && flag == true)
		return ("/usr/bin");
	return (NULL);
}

int	morethan2arg(char **command_args)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (command_args[i])
	{
		i++;
		counter++;
	}
	return (counter);
}
