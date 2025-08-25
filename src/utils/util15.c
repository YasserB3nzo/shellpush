/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util15.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 01:59:35 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/25 00:37:08 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_cmd_not_found(char **com, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	if (((com[0][0] == 39 && com[0][1] == 39) || (com[0][0] == 34
				&& com[0][1] == 34)) && com[0][2] == '\0')
		ft_putstr_fd("'': command not found\n", 2);
	else
	{
		ft_putstr_fd(com[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	senv_clear(&data->list_env);
	free_array(data->env);
	exit(127);
}

void	execute_command_part_one(char **com, t_command *command, t_data *data,
		char *path)
{
	int	red;

	red = 0;
	signal(SIGINT, SIG_DFL);
	free(data->line);
	if (command->infile || command->outfile)
		red = hand_the_redirectionin(command);
	if (red == 1)
	{
		senv_clear(&data->list_env);
		free_array(data->env);
		exit(1);
	}
	else if (path == NULL && get_command_in_one_char(com) == 0)
	{
		if (check_file_exists_no_exec(com[0]))
			print_no_exec_error(com, data);
		else
			print_cmd_not_found(com, data);
	}
}

void	execute_command_part_two(t_command *command, t_data *data)
{
	if (command->prev && !command->infile)
	{
		dup2(data->fd_in, STDIN_FILENO);
		ft_close(data->fd_in, "fd_in\n");
	}
	if (command->next && !command->outfile)
		dup2(data->fd[1], STDOUT_FILENO);
	if (command->next)
	{
		ft_close(data->fd[1], "fd[0]\n");
		ft_close(data->fd[0], "fd[1]\n");
	}
}

void	execute_command_part_three(char **com, t_command *command, t_data *data,
		char *path)
{
	int	cmd;

	cmd = get_command_in_one_char(com);
	if (cmd == 0)
		exec_external(com, command, data, path);
	else
		run_builtins(cmd, command, data, 1);
}
