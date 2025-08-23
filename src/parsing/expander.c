/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:42:43 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/22 15:08:17 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	check_ex(char *str, int size)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	if (size == 0)
		return (true);
	while (str[i] && i < size)
	{
		if (str[i] == 34)
			flag++;
		i++;
	}
	if (flag % 2 == 0)
		return (true);
	return (false);
}

char	**get_vars_content(char **var, char **env, char *str, int i)
{
	char	**vars;
	int		k;

	k = 0;
	vars = malloc(sizeof(char *) * (array_size(var) + 1));
	while (var[i])
	{
		while (str[k] && str[k] != '$')
		{
			k++;
			if (str[k] == '$' && str[k + 1] == '$')
				k += 2;
		}
		if (var[i][0] == '?' && var[i][1] == '\0')
			vars[i] = ft_itoa(g_signal.ret);
		else if (check_back_for_heredoc(str, k) == true)
			vars[i] = ft_strjoin("$", var[i]);
		else
			vars[i] = get_env_variable_value(env, var[i]);
		i++;
		k++;
	}
	vars[i] = NULL;
	free_array(var);
	return (vars);
}

char	*join_vars(char **vars)
{
	char	*line;
	int		i;
	int		j;
	int		size;

	i = 0;
	size = 0;
	j = 0;
	while (vars[i])
		size += ft_strlen(vars[i++]);
	line = malloc(sizeof(char) * (size + 1));
	i = 0;
	size = 0;
	while (vars[i])
	{
		j = 0;
		while (vars[i][j])
			line[size++] = vars[i][j++];
		i++;
	}
	line[size] = '\0';
	return (line);
}

char	*expand_variable(char *str, t_data *data)
{
	char	*result;

	if (!str || !data || !data->env)
		return (str);
	result = simple_expand_variable(str, data->env);
	if (result != str)
		free(str);
	return (result);
}

char	*check_expand(char *str, t_data *data)
{
	char	*new_str;

	new_str = expand_variable(str, data);
	return (new_str);
}
