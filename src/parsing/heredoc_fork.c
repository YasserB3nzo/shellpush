/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 02:28:39 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	fork_and_wait(int pid, int status, int fd0, char *line)
{
	waitpid(pid, &status, 0);
	g_signal.ff = 0;
	dup2(fd0, 0);
	close(fd0);
	free(line);
	return (status);
}

int	prepare_and_fork(t_cmds *cmds, bool *flag, char **line, int *fd0)
{
	*fd0 = dup(0);
	check_quot_and_filename(flag, line, cmds->cmd[0]);
	g_signal.ff = 1;
	return (ft_fork());
}
