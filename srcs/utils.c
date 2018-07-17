/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 05:25:30 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/17 20:11:05 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	get_hexa(char *ptr, long nb, int count, int size)
{
	if (nb >= 16)
		get_hexa(ptr, nb / 16, count + 1, size);
	if (nb % 16 < 10)
		ptr[size - 1 - count] = (nb % 16) + 48;
	else
		ptr[size - 1 - count] = (nb % 16) + 87;
}

void		print_one_hexa(unsigned char nb, char first)
{
	if (first && (nb < 16 || !nb))
		ft_putchar('0');
	if (nb >= 16)
		print_one_hexa(nb / 16, 0);
	if (nb % 16 < 10)
		ft_putchar((nb % 16) + 48);
	else
		ft_putchar((nb % 16) + 87);
}

void		put_hexa(long nb, int size)
{
	char	*num;

	if (!(num = (char*)malloc(size + 1)))
		ft_exiterror("Malloc failed", 1);
	ft_memset(num, '0', size);
	num[size] = '\0';
	get_hexa(num, nb, 0, size);
	ft_putstr(num);
	free(num);
}
