/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellerr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:42:04 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/25 01:02:50 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_env_if_plus(t_env *env_node, char *export_value)
{
	char	*orig;

	orig = env_node->var_name;
	if (orig[ft_strlen(orig) - 1] != '=' && check_eq(orig) == true)
		env_node->var_name = ft_strjoin3(orig, '=', export_value);
	else
		env_node->var_name = ft_strjoin(orig, export_value);
	free(orig);
}

static int	count_and_validate_heredocs(t_cmds *curr, int heredoc_num)
{
	while (curr)
	{
		if (heredoc_num > 4)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token", 2);
			ft_putstr_fd(" `<<<'\n", 2);
			return (2);
		}
		if (curr->token == HereDoc)
			heredoc_num++;
		curr = curr->next;
	}
	return (0);
}

int	process_heredoc_delimiters(t_cmds *head, int i)
{
	while (head && i != 130)
	{
		if (head->token == HereDocDel)
		{
			if (ft_strcmp(head->cmd[0], "''") == 0 || ft_strcmp(head->cmd[0],
					"\"\"") == 0)
			{
				free(head->cmd[0]);
				head->cmd[0] = ft_strdup("");
			}
			i = open_heredoc(head, 0, 0, true);
		}
		head = head->next;
	}
	return (i);
}

int	heredoc(t_cmds *head, t_cmds *curr, int i, int heredoc_num)
{
	int	status;

	status = count_and_validate_heredocs(curr, heredoc_num);
	if (status != 0)
		return (status);
	return (process_heredoc_delimiters(head, i));
}

int	cmdcheck(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}
