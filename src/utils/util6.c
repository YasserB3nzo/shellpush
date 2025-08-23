/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:56:30 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/23 18:38:47 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*handle_variable_expansion(char *str, int *i, char **env)
{
	char	*var_start;
	char	*var_end;
	char	*var_name;
	char	*var_value;

	var_start = &str[*i + 1];
	var_end = var_start;
	if (*var_start == '?')
	{
		var_end++;
		var_name = ft_strdup("?");
	}
	else
	{
		while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
			var_end++;
		var_name = ft_substr(var_start, 0, var_end - var_start);
	}
	if (ft_strcmp(var_name, "?") == 0)
		var_value = ft_itoa(g_signal.ret);
	else
		var_value = get_env_variable_value(env, var_name);
	free(var_name);
	*i = var_end - str;
	return (var_value);
}

static int	handle_quotes_and_expansion(char *str, int *i, char **env,
	t_expansion_data *data)
{
	static int	in_single = 0;
	static int	in_double = 0;
	char		*var_value;
	int			j;

	if (str[*i] == '\'' && !in_double)
		in_single = !in_single;
	else if (str[*i] == '"' && !in_single)
		in_double = !in_double;
	if (!in_single && str[*i] == '$' && str[*i + 1]
		&& (ft_isalnum(str[*i + 1]) || str[*i + 1] == '?'))
	{
		var_value = handle_variable_expansion(str, i, env);
		if (var_value)
		{
			j = 0;
			while (var_value[j])
				data->result[(*data->result_pos)++] = var_value[j++];
			free(var_value);
		}
		return (1);
	}
	return (0);
}

char	*simple_expand_variable(char *str, char **env)
{
	char				*result;
	int					i;
	int					result_pos;
	t_expansion_data	exp_data;

	if (!str || !env)
		return (str);
	result = malloc(ft_strlen(str) + 1000);
	if (!result)
		return (str);
	result_pos = 0;
	i = 0;
	exp_data.result = result;
	exp_data.result_pos = &result_pos;
	while (str[i])
	{
		if (handle_quotes_and_expansion(str, &i, env, &exp_data))
			continue ;
		result[result_pos++] = str[i++];
	}
	result[result_pos] = '\0';
	return (result);
}

char	*get_env_variable_value(char **env, char *variable_name)
{
	int	i;
	int	var_name_length;

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
