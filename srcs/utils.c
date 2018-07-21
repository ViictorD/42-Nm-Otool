/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 05:25:30 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 15:53:59 by vdarmaya         ###   ########.fr       */
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

void		check_corrupted(struct nlist_64 *array, unsigned int nsyms, \
				unsigned int filesize)
{
	unsigned int	i;

	i = 0;
	if (nsyms >= filesize)
		ft_exiterror("Binary corrupted", 1);
	while (i < nsyms)
	{
		if (swap_bits(array[i].n_un.n_strx, 32) > filesize)
			ft_exiterror("Binary corrupted", 1);
		++i;
	}
}

void		check_corrupted_32(struct nlist *array, unsigned int nsyms, \
				unsigned int filesize)
{
	unsigned int	i;

	i = 0;
	while (i < nsyms)
	{
		if (swap_bits(array[i].n_un.n_strx, 32) > filesize)
			ft_exiterror("Binary corrupted", 1);
		++i;
	}
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
