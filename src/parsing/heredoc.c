/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:16:35 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_herd(int pid)
{
	(void)pid;
	printf("\n");
	exit(130);
}

int	print_error(int k, char *str)
{
	char	*num;

	num = ft_itoa(k);
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putstr_fd(num, 2);
	free(num);
	ft_putstr_fd("  delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("')\n", 2);
	return (0);
}

void	check_quot_and_filename(bool *flag, char **filename, char *str)
{
	static int	i;
	char		*tmp1;

	i++;
	if (check_quot(str) != 0)
		*flag = false;
	tmp1 = ft_itoa(i);
	*filename = ft_strjoin("/tmp/HereDoc", tmp1);
	free(tmp1);
}

int	open_heredoc(t_cmds *cmds, int pid, int status, bool flag)
{
	char	*line;
	int		fd0;

	pid = prepare_and_fork(cmds, &flag, &line, &fd0);
	if (pid == 0)
		child(line, cmds, flag);
	else if (pid < 0)
	{
		ft_putstr_fd("minishell: fork fail while creating the HereDoc\n", 2);
		free(line);
		close(fd0);
		return (-1);
	}
	status = fork_and_wait(pid, status, fd0, cmds->cmd[0]);
	cmds->cmd[0] = line;
	cmds->prev->token = Input;
	cmds->token = Infile;
	if (status != 0)
		return (130);
	return (0);
}
