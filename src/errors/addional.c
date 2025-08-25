/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addional.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:26:35 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/25 00:59:43 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_no_exec_error(char **com, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(com[0], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	senv_clear(&data->list_env);
	free_array(data->env);
	exit(127);
}

bool	check_file_exists_no_exec(char *cmd)
{
	struct stat	stats;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &stats) == 0 && !S_ISDIR(stats.st_mode))
		{
			if (access(cmd, X_OK) != 0)
				return (true);
		}
	}
	return (false);
}
