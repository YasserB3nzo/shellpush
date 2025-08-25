/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util13.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:07:52 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_external(char **com, t_command *command, t_data *data, char *path)
{
	struct stat	stats;

	(void)command;
	if (signal(SIGQUIT, printsignalsc) != SIG_ERR)
		g_signal.ret = 131;
	execve(path, com, data->env);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(com[0], 2);
	ft_putstr_fd(": ", 2);
	if (stat(path, &stats) == 0 && S_ISDIR(stats.st_mode))
	{
		ft_putendl_fd("Is a directory", 2);
		senv_clear(&data->list_env);
		free_array(data->env);
		exit(126);
	}
	ft_putendl_fd(strerror(errno), 2);
	senv_clear(&data->list_env);
	free_array(data->env);
	if (errno == EACCES)
		exit(126);
	else
		exit(127);
}
