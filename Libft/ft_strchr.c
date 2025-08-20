/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:39:27 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:19:43 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	if (!str)
		return (NULL);
	else
	{
		while (*str)
		{
			if (*str == (char)c)
				return ((char *)str);
			str++;
		}
		if (*str == (char)c)
			return ((char *)str);
		return (NULL);
	}
}

/*
#include <stdio.h>

//#include <string.h>

int	main(void)
{
	const char	*str = "hello";
	char		ch;
	char		*result;

	ch = 'e';
	result = ft_strchr(str, ch);
	printf("Character '%s'", result);
	return (0);
}*/
