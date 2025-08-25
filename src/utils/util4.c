/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:15:30 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_command_in_one_char(char **str)
{
	int	c;

	c = 0;
	if (!str || !*str)
		return (c);
	if (ft_strcmp(str[0], "cd") == 0)
		c = 1;
	else if (ft_strcmp(str[0], "pwd") == 0)
		c = 2;
	else if (ft_strcmp(str[0], "env") == 0 && str[1] == NULL)
		c = 3;
	else if (ft_strcmp(str[0], "export") == 0)
		c = 4;
	else if (ft_strcmp(str[0], "unset") == 0)
		c = 5;
	else if (ft_strcmp(str[0], "exit") == 0)
		c = 6;
	else if (ft_strcmp(str[0], "echo") == 0)
		c = 7;
	else if (ft_strcmp(str[0], "setenv") == 0)
		c = 8;
	else if (ft_strcmp(str[0], "unsetenv") == 0)
		c = 9;
	return (c);
}

int	execute_command(t_env *list, t_command *command, t_data *data, int index)
{
	char	*path;

	if (!command->cmd)
		return (-1);
	if (command->cmd[0][0] == '\0' && !command->infile && !command->outfile)
		return (0);
	path = get_my_path(list, command->cmd, data->path_flag, 0);
	signal(SIGINT, SIG_IGN);
	data->pid[index] = ft_fork();
	if (data->pid[index] == 0)
	{
		execute_command_part_one(command->cmd, command, data, path);
		execute_command_part_two(command, data);
		if (command->cmd[0][0] != '\0')
			execute_command_part_three(command->cmd, command, data, path);
		senv_clear(&data->list_env);
		free_array(data->env);
		exit(0);
	}
	free(path);
	if (data->pid[index] < 0)
		return (-1);
	return (0);
}
