/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:57:49 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/23 16:57:53 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_puterror_fd(char *str1, char *str2)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
}

void	print_value(char *str)
{
	while (str && *str && *str != '=')
	{
		printf("%c", *str);
		str++;
	}
	if (str && *str)
	{
		printf("=\"");
		str++;
		while (str && *str)
		{
			printf("%c", *str);
			str++;
		}
		printf("\"");
	}
	printf("\n");
}

char	*increment_s1(char *s1)
{
	if (*s1 == 39)
	{
		s1++;
		while (*s1 && *s1 != 39)
			s1++;
	}
	else
		while (*s1 == '$' && s1[1])
			s1++;
	return (s1);
}

bool	is_it_inside(char *str)
{
	while (str && *str)
	{
		if (*str == '=')
			return (true);
		str++;
	}
	return (false);
}
