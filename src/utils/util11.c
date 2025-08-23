/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util11.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:58:26 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/23 18:26:57 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	escape_dollars_in_single_quotes(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == 34)
			i = check_double(cmd, i);
		else if (cmd[i] == 39)
		{
			i++;
			while (cmd[i] && cmd[i] != 39)
			{
				if (cmd[i] == '$')
					cmd[i] = '1';
				i++;
			}
			if (cmd[i] == 39)
				i++;
		}
		else
			i++;
	}
}

bool	check_eq(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '=')
			j++;
		i++;
	}
	if (j == 0)
		return (true);
	return (false);
}

void	init_line_data(t_line *line_data, char **lines, char **vars, char *cmd)
{
	line_data->i = 0;
	line_data->k = 0;
	line_data->size = 0;
	line_data->pos = 0;
	line_data->line = malloc(sizeof(char) * (calculate_total_string_length(vars,
					lines) + 1));
	escape_dollars_in_single_quotes(cmd);
}

char	*get_final_line(char **lines, char **vars, char *cmd, t_line *data)
{
	init_line_data(data, lines, vars, cmd);
	while (cmd[data->size])
	{
		if ((cmd[data->size] != '$' || cmd[data->size + 1] != '$')
			&& lines[data->k])
		{
			data->pos = copy_string_to_buffer(data->line, lines[data->k++],
					data->pos);
			while (cmd[data->size] && cmd[data->size] != '$')
				data->size++;
		}
		else if (cmd[data->size] == '$' && vars[data->i]
			&& vars[data->i][0] != '\0')
			process_variable_expansion(cmd, vars, data);
		else if (cmd[data->size])
			data->size++;
	}
	return (data->line[data->pos] = '\0', data->line);
}
