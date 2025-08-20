/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:39:23 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:17:36 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	char	*result;

	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	result = malloc(nmemb * size);
	if (!result)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
		result[i++] = 0;
	return (result);
}
