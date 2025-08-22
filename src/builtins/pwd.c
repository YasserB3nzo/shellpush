/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 18:40:02 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/21 15:47:15 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	change_directory(t_env *env_list, char **command_args)
{
	char	*home_path;
	int		arg_count;

	arg_count = array_size(command_args);
	if (arg_count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	home_path = find_variable_value(env_list, env_list, "HOME", false);
	if (command_args[1] == NULL || command_args[1][0] == '~')
	{
		if (!home_path)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		else
			change_directory_path(env_list, home_path);
	}
	else if (command_args[1][0] == '-' && command_args[1][1] == '\0')
	{
		char *oldpwd = find_variable_value(env_list, env_list, "OLDPWD", false);
		if (!oldpwd)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		else {
			change_directory_path(env_list, oldpwd);
			ft_putstr_fd(oldpwd, 1);
			ft_putchar_fd('\n', 1);
		}
	}
	else
		change_directory_path(env_list, command_args[1]);
}

void	print_working_directory(t_env *env_list)
{
	char	buffer[PATH_MAX];
	char	*current_path;
	char	*pwd_value;

	current_path = getcwd(buffer, PATH_MAX);
	if (current_path)
	{
		ft_putstr_fd(current_path, 1);
		ft_putchar_fd('\n', 1);
	}
	else
	{
		pwd_value = find_variable_value(env_list, env_list, "PWD", false);
		ft_putstr_fd(pwd_value, 1);
		ft_putchar_fd('\n', 1);
	}
}

t_env	*remove_node(t_env *head, t_env *node_to_remove)
{
	if (!head || !node_to_remove)
		return (head);
	if (node_to_remove == head)
	{
		head = node_to_remove->next;
		if (head)
			head->prev = NULL;
		free(node_to_remove->var_name);
		free(node_to_remove);
		return (head);
	}
	if (node_to_remove->prev)
		node_to_remove->prev->next = node_to_remove->next;
	if (node_to_remove->next)
		node_to_remove->next->prev = node_to_remove->prev;
	free(node_to_remove->var_name);
	free(node_to_remove);
	return (head);
}

t_env	*unset_env(t_env *list, char **command_args, t_data *data)
{
	t_env	*env_node;
	int		i;

	i = 0;
	env_node = NULL;
	while (command_args[i])
	{
		if (is_it_inside(command_args[i]) == false)
		{
			if (data->path_flag == true && ft_strcmp(command_args[i],
					"PATH") == 0)
				data->path_flag = false;
			env_node = find_env_variable(list, command_args[i]);
			if (env_node)
				list = remove_node(list, env_node);
		}
		i++;
	}
	return (list);
}

int	is_numeric(char *str)
{
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (1);
		str++;
	}
	return (0);
}
