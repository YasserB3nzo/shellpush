/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:42:04 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/21 15:42:08 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_env_if_plus(t_env *env_node, char *export_value)
{
	char	*original_var;

	original_var = env_node->var_name;
	if (original_var[ft_strlen(original_var) - 1] != '='
		&& check_eq(original_var) == true)
		env_node->var_name = ft_strjoin3(original_var, '=', export_value);
	else
		env_node->var_name = ft_strjoin(original_var, export_value);
	free(original_var);
}

int	ft_strcmp_for_heredoc(char *s1, char *s2)
{
	char	*str;
	int		k;
	int		i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	str = get_string(ft_strdup(s2), 0, 0, get_size(s2));
	while (s1[i] == str[i] && s1[i] != '\0' && str[i] != '\0')
		i++;
	k = s1[i] - str[i];
	free(str);
	return (k);
}

int	heredoc(t_cmds *head, t_cmds *curr, int i, int heredoc_num)
{
	while (curr)
	{
		if (heredoc_num > 16)
			ft_putstr_fd("minishell: maximum here-doc count exceeded\n", 2);
		if (heredoc_num > 16)
			exit(2);
		if (curr->token == HereDoc)
			heredoc_num++;
		curr = curr->next;
	}
	while (head && i != 130)
	{
		if (head->token == HereDocDel)
		{
			if (ft_strcmp(head->cmd[0], "\'\'") == 0 || ft_strcmp(head->cmd[0],
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

int	errors_managment(t_data *data, int i)
{
	t_cmds	*curr;
	t_cmds	*head;

	curr = data->lst;
	head = curr;
	i = heredoc(head, head, i, 0);
	while (curr && i == 0)
	{
		if (curr->token == Pipe)
			i = check_for_pipe(curr);
		else if (curr->token == Output || curr->token == Input)
			i = check_for_in_out_put(curr);
		else if (curr->token == Append || curr->token == HereDoc)
			i = check_for_append_heredoc(curr);
		curr = curr->next;
	}
	return (i);
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
