/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:39:23 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:18:12 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_to_result(char *result, long long nb, int total_len)
{
	int	i;

	i = total_len;
	while (nb)
	{
		result[i--] = (nb % 10) + '0';
		nb /= 10;
	}
}

static int	ft_len(long long n)
{
	int	len_total;

	len_total = 0;
	while (n > 0)
	{
		len_total++;
		n /= 10;
	}
	return (len_total);
}

static char	*is_zero(void)
{
	char	*result;

	result = malloc(2);
	if (result == NULL)
		return (NULL);
	result[0] = '0';
	result[1] = '\0';
	return (result);
}

char	*ft_itoa(int n)
{
	int			total_len;
	char		*result;
	long long	nb;

	nb = n;
	if (n == 0)
		return (is_zero());
	if (n < 0)
	{
		nb = -nb;
		total_len = ft_len(nb) + 1;
	}
	else
		total_len = ft_len(nb);
	result = malloc(sizeof(char) * (total_len + 1));
	if (!result)
		return (NULL);
	if (n < 0)
		result[0] = '-';
	ft_putnbr_to_result(result, nb, total_len - 1);
	result[total_len] = '\0';
	return (result);
}
/*
#include <stdio.h>

int	main(void)
{
	char *str;

	// Test 1: Positive number
	str = ft_itoa(12345);
	printf("Test 1: %s\n", str);
	free(str);

	// Test 2: Negative number
	str = ft_itoa(-12345);
	printf("Test 2: %s\n", str);
	free(str);

	// Test 3: Zero
	str = ft_itoa(0);
	printf("Test 3: %s\n", str);
	free(str);

	// Test 4: Negative number - smallest negative integer (INT_MIN)
	str = ft_itoa(INT_MIN);
	printf("Test 4: %s\n", str);
	free(str);

	// Test 5: Positive number - maximum integer value (INT_MAX)
	str = ft_itoa(INT_MAX);
	printf("Test 5: %s\n", str);
	free(str);

	// Test 6: Large positive number
	str = ft_itoa(987654321);
	printf("Test 6: %s\n", str);
	free(str);

	// Test 7: Large negative number
	str = ft_itoa(-987654321);
	printf("Test 7: %s\n", str);
	free(str);

	return (0);
}*/
