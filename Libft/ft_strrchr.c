/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:25:53 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:21:12 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;
	char	*str;

	len = ft_strlen(s);
	str = (char *)s + len;
	if (*str == (char)c)
		return (str);
	str--;
	while (str >= s)
	{
		if (*str == (char)c)
			return (str);
		str--;
	}
	return (NULL);
}
