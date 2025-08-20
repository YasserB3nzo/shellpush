/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:17:29 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/04/12 21:17:31 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t len)
{
	unsigned char	*s1;

	s1 = s;
	while (len > 0)
		s1[--len] = 0;
}
/*
#include <stdio.h>

int	main(void)
{
	char	arr[10];

			arr[10] = "mikemok";
	ft_bzero(arr, sizeof(arr));
	for (int i = 0; i < 10; i++)
		printf("%d", arr[i]);
	return (0);
}
*/
