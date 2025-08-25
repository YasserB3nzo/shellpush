/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 01:35:50 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	write_line(int fd, char *line, t_cmds *cmds, bool flag)
{
	if (flag == true)
		line = expand_variable(line, cmds->data);
	ft_putstr_fd(line, fd);
	ft_putchar_fd('\n', fd);
	free(line);
}

bool	is_delimiter(char *line, t_cmds *cmds)
{
	if (line && ft_strcmp_for_heredoc(line, cmds->cmd[0]) == 0)
	{
		free(line);
		return (true);
	}
	return (false);
}

void	read_loop(int fd, char *line, t_cmds *cmds, bool flag)
{
	int	k;

	k = 1;
	while (1)
	{
		if (!line && print_error(k, cmds->cmd[0]) == 0)
			break ;
		else if (is_delimiter(line, cmds))
			break ;
		k++;
		write_line(fd, line, cmds, flag);
		line = readline(">");
	}
}

void	child(char *line, t_cmds *cmds, bool flag)
{
	int	fd;

	fd = open(line, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	free(line);
	signal(SIGINT, signal_herd);
	line = readline(">");
	read_loop(fd, line, cmds, flag);
	close(fd);
	senv_clear(&cmds->data->list_env);
	if (cmds->data->env)
		free_array(cmds->data->env);
	exit(0);
}
