/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:39:27 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:19:23 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	nb;

	if (fd == -1)
		return ;
	if (n < 0)
	{
		nb = -n;
		ft_putchar_fd('-', fd);
	}
	else
		nb = n;
	if (nb >= 10)
		ft_putnbr_fd(nb / 10, fd);
	ft_putchar_fd(nb % 10 + '0', fd);
}
/*
int	main(void)
{
	int	fd;

	fd = open("yassin.txt", O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		return (1);
	}
	ft_putnbr_fd(1000000, fd);
	close(fd);
	return (0);
}
*/
