/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util14.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:30:11 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/25 02:10:42 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**alloc_res(void)
{
	char	**res;

	res = (char **)malloc(sizeof(char *) * 3);
	if (!res)
		return (NULL);
	res[0] = NULL;
	res[1] = NULL;
	res[2] = NULL;
	return (res);
}

static void	fill_key_value(char *str, int pos, char **res)
{
	if (!str[pos])
	{
		res[0] = ft_strdup(str);
		res[1] = NULL;
	}
	else
	{
		res[0] = ft_substr(str, 0, pos);
		if (str[pos + 1])
			res[1] = ft_substr(str, pos + 1, ft_strlen(str + pos + 1));
		else
			res[1] = NULL;
	}
}

char	**get_key_and_value(char *str, char **ptr, int i, int j)
{
	int		pos;
	char	**res;

	(void)ptr;
	(void)i;
	(void)j;
	pos = 0;
	while (str[pos] && str[pos] != '=')
		pos++;
	res = alloc_res();
	if (!res)
		return (NULL);
	fill_key_value(str, pos, res);
	return (res);
}
