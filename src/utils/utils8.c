/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:11:49 by ksohail-          #+#    #+#             */
/*   Updated: 2025/08/20 18:16:47 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	check_next_for_both(char *str)
{
	int	i;
	int	double_quote_count;
	int	single_quote_count;

	i = 0;
	double_quote_count = 0;
	single_quote_count = 0;
	while (str[i])
	{
		if (str[i] == 34)
			double_quote_count++;
		if (str[i] == 39)
			single_quote_count++;
		i++;
	}
	if ((double_quote_count != 0 && double_quote_count % 2 == 0)
		|| (single_quote_count != 0 && single_quote_count % 2 == 0))
		return (true);
	if (double_quote_count == 0 && single_quote_count == 0)
		return (true);
	return (false);
}

int	get_2d_size(char **vars, char **lines)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (vars[i])
		size += ft_strlen(vars[i++]);
	i = 0;
	while (lines[i])
		size += ft_strlen(lines[i++]);
	return (size);
}

int	array_size(char **var)
{
	int	i;

	i = 0;
	while (var[i])
		i++;
	return (i);
}

char	**linked_list_to_array(t_env *list)
{
	char	**array;
	t_env	*current;
	int		count;
	int		i;

	i = 0;
	count = count_nodes(list);
	array = (char **)malloc((count + 1) * sizeof(char *));
	if (array == NULL)
		exit(EXIT_FAILURE);
	current = list;
	while (i < count)
	{
		array[i] = ft_strdup(current->var_name);
		if (array[i] == NULL)
		{
			perror("strdup");
			free_array(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

bool	check_back_for_heredoc(char *str, int index)
{
	if (index < 0 || !str[index])
		return (false);
	index--;
	while (index >= 0 && str[index] == ' ')
		index--;
	if (index >= 1 && str[index] == '<')
	{
		index--;
		if (index >= 0 && str[index] == '<')
			return (true);
	}
	return (false);
}
