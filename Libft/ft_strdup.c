/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:39:28 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:19:48 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*result;
	char	*ptr;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen((char *)s) + 1));
	ptr = result;
	if (!result)
		return (NULL);
	while (*s)
		*ptr++ = *s++;
	*ptr = '\0';
	return (result);
}
