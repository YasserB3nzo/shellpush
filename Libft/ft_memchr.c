/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:39:23 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:18:44 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	sh;

	i = 0;
	str = (unsigned char *)s;
	sh = (unsigned char)c;
	while (i < n)
	{
		if (str[i] == sh)
			return ((void *)&str[i]);
		i++;
	}
	return (NULL);
}
