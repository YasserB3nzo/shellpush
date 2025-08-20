/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:33:14 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:22:37 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	total_len;

	if (!s)
		return (NULL);
	if (start >= (unsigned int)ft_strlen(s))
	{
		result = (char *)(malloc(sizeof(char)));
		if (!result)
			return (NULL);
		result[0] = '\0';
		return (result);
	}
	total_len = ft_strlen((char *)(s + start));
	if (total_len > len)
		total_len = len;
	result = (char *)(malloc(sizeof(char) * (total_len + 1)));
	if (!result)
		return (NULL);
	ft_strlcpy(result, (char *)(s + start), total_len + 1);
	return (result);
}
