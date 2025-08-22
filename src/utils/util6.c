/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:10:27 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/22 15:10:28 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*simple_expand_variable(char *str, char **env)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i, j, result_pos;
	int		result_len;

	if (!str || !env)
		return (str);
	result_len = ft_strlen(str) + 1000;
	result = malloc(result_len);
	if (!result)
		return (str);
	result_pos = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			i++; // Skip $
			j = i;
			if (str[i] == '?')
			{
				i++;
				var_name = ft_strdup("?");
			}
			else
			{
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
				var_name = ndup(&str[j], i - j);
			}
			if (ft_strcmp(var_name, "?") == 0)
				var_value = ft_itoa(g_signal.ret);
			else
				var_value = get_env_variable_value(env, var_name);
			if (var_value)
			{
				j = 0;
				while (var_value[j] && result_pos < result_len - 1)
					result[result_pos++] = var_value[j++];
				free(var_value);
			}
			free(var_name);
		}
		else
		{
			if (result_pos < result_len - 1)
				result[result_pos++] = str[i];
			i++;
		}
	}
	result[result_pos] = '\0';
	return (result);
}

char	*get_env_variable_value(char **env, char *variable_name)
{
	int		i;
	int		var_name_length;

	i = 0;
	if (variable_name == NULL)
		return (NULL);
	var_name_length = ft_strlen(variable_name);
	while (env[i])
	{
		if (ft_strncmp(env[i], variable_name, var_name_length) == 0
			&& env[i][var_name_length] == '=')
			return (ft_strdup(env[i] + var_name_length + 1));
		i++;
	}
	return (ft_strdup(""));
}

int	copy_string_to_buffer(char *line, char *ptr, int pos)
{
	int	i;
	int	tpos;

	i = 0;
	tpos = pos;
	if (ptr)
	{
		while (ptr[i])
		{
			line[tpos++] = ptr[i++];
		}
	}
	return (tpos);
}

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

void	init_line_data(t_line *line_data, char **lines, char **vars, char *cmd)
{
	line_data->i = 0;
	line_data->k = 0;
	line_data->size = 0;
	line_data->pos = 0;
	line_data->line = malloc(sizeof(char)
			* (calculate_total_string_length(vars, lines) + 1));
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
			data->pos = copy_string_to_buffer(data->line,
					lines[data->k++], data->pos);
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
