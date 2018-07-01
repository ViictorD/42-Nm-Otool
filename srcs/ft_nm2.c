/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 22:23:18 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/01 16:11:24 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	get_hexa(char *ptr, long nb, int count)
{
	if (nb >= 16)
		get_hexa(ptr, nb / 16, count + 1);
	if (nb % 16 < 10)
		ptr[15 - count] = (nb % 16) + 48;
	else
		ptr[15 - count] = (nb % 16) + 87;
}

void		put_hexa(long nb)
{
	char	*num;

	num = (char*)malloc(17);
	ft_memset(num, '0', 16);
	num[16] = '\0';
	get_hexa(num, nb, 0);
	ft_putstr(num);
	free(num);
}

static void	print_output2(t_sorted *index, struct nlist_64 *array, int i)
{
	if (array[index->count[i]].n_type == 14)
	{
		if (array[index->count[i]].n_sect == '\t')
			ft_putstr(" d ");
		else if (array[index->count[i]].n_sect == '\n')
			ft_putstr(" b ");
		else
			ft_putstr(" t ");
	}
	else if (array[index->count[i]].n_type == 15 && \
			array[index->count[i]].n_sect == '\b')
		ft_putstr(" D ");
	else
		ft_putstr(" T ");
}

void		print_output(t_sorted *index, struct nlist_64 *array, \
				char *string_table)
{
	int		i;
	int		max;

	max = index->tab_size;
	i = -1;
	while (++i <= max)
	{
		if (array[index->count[i]].n_type == 14 || \
			array[index->count[i]].n_type == 15 || \
			array[index->count[i]].n_desc == 16)
		{
			put_hexa(array[index->count[i]].n_value);
			print_output2(index, array, i);
			ft_putendl(string_table + array[index->count[i]].n_un.n_strx);
		}
		else if (array[index->count[i]].n_type == N_EXT)
		{
			ft_putstr("                 U ");
			ft_putendl(string_table + array[index->count[i]].n_un.n_strx);
		}
	}
}
