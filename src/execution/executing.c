/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:03:16 by ybenzidi-          #+#    #+#             */
/*   Updated: 2025/08/20 18:14:19 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	change_underscore(t_data *data, t_command *command, char *str, int i)
{
	t_env	*env_node;

	if (!command->cmd || !command->cmd[0])
		return (false);
	while (command->cmd[i])
		i++;
	str = command->cmd[--i];
	if (!str || *str == '\n')
		return (false);
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
	return (true);
}

int	run_builtins(int cmd_id, t_command *command, t_data *data, int flag)
{
	t_env	*env_list;

	env_list = data->list_env;
	if (cmd_id == 1)
		change_directory(env_list, command->cmd);
	else if (cmd_id == 2)
		print_working_directory(env_list);
	else if (cmd_id == 3)
		print_environment(env_list);
	else if (cmd_id == 4)
		export(env_list, command->cmd, '-', 1);
	else if (cmd_id == 5)
		data->list_env = unset_env(env_list, command->cmd, data);
	else if (cmd_id == 6)
		exit_myminishell(command->cmd, flag);
	else if (cmd_id == 7)
		ft_echo(command->cmd + 1, true, 0);
	return (0);
}

void	only_builtins(t_data *data, t_command *list, int builtin_command)
{
	int	in;
	int	out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	if (list->infile || list->outfile)
		g_signal.ret = hand_the_redirectionin(list);
	if (g_signal.ret != 1)
		run_builtins(builtin_command, list, data, 0);
	if ((g_signal.ret != 1) && list->infile)
		dup2(in, STDIN_FILENO);
	else
		ft_close(in, "in");
	if ((g_signal.ret != 1) && list->outfile)
		dup2(out, STDOUT_FILENO);
	else
		ft_close(out, "out");
}

void	with_pipe(t_data *data, t_command *list)
{
	data->k = 0;
	data->pid = malloc(sizeof(int) * (get_command_size(list)));
	data->fd_in = STDIN_FILENO;
	while (list)
	{
		if (list->next)
		{
			if (pipe(data->fd) == -1)
				break ;
		}
		g_signal.ret = execute_command(data->list_env, list, data, data->k++);
		if (list->next)
			ft_close(data->fd[1], "data->fd[0]");
		if (list->prev)
			ft_close(data->fd_in, "data->fd[1]");
		if (list->next)
			data->fd_in = data->fd[0];
		list = list->next;
	}
	if (g_signal.ret == 0 && data->k != 0)
		g_signal.ret = wait_pid(data->pid, data->k);
	free(data->pid);
	data->pid = NULL;
}

int	executing(t_data *data)
{
	t_command	*command_list;
	int			builtin_command;
	bool		flag;

	command_list = data->list;
	flag = false;
	builtin_command = get_command_in_one_char(command_list->cmd);
	if (!command_list || (command_list->cmd && command_list->cmd[0][0] == '\n'))
		return (2);
	if (!command_list->next)
		flag = change_underscore(data, command_list, NULL, 0);
	if (builtin_command != 0 && !command_list->next)
		only_builtins(data, command_list, builtin_command);
	else
		with_pipe(data, command_list);
	if (flag == true)
		change_underscore(data, command_list, NULL, 0);
	g_signal.ret_exit = g_signal.ret;
	return (g_signal.ret);
}
