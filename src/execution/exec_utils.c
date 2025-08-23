/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:58:26 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/23 18:38:47 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_builtin_command(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "echo") == 0)
		return (true);
	return (false);
}

static void	update_underscore_env(t_data *data, char *str)
{
	t_env	*env_node;

	env_node = find_env_variable(data->list_env, "_");
	if (env_node)
	{
		free(env_node->var_name);
		env_node->var_name = ft_strjoin("_=", str);
	}
	else
	{
		env_node = env_new(data->list_env, ft_strjoin("_=", str));
		data->list_env = env_last(data->list_env);
		if (data->list_env)
			data->list_env->next = env_node;
		else
			data->list_env = env_node;
	}
}

bool	change_underscore(t_data *data, t_command *command, char *str, int i)
{
	if (!command->cmd || !command->cmd[0])
		return (false);
	if (is_builtin_command(command->cmd[0]))
		return (false);
	while (command->cmd[i])
		i++;
	str = command->cmd[--i];
	if (!str || *str == '\n')
		return (false);
	update_underscore_env(data, str);
	return (true);
}
