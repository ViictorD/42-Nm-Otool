/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 22:23:18 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/02 05:29:17 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char	find_seg_sym(int nb, char **arr)
{
	if (!nb)
		return ('S');
	--nb;
	if (!ft_strcmp(arr[nb], SECT_TEXT))
		return ('T');
	else if (!ft_strcmp(arr[nb], SECT_DATA))
		return ('D');
	else if (!ft_strcmp(arr[nb], SECT_BSS))
		return ('B');
	else
		return ('S');
}

static void	print_output2(t_sorted *index, struct nlist_64 *array, int i, \
					char **arr)
{
	char type;
	char c;

	type = array[index->count[i]].n_type & N_TYPE;
	c = 'U';
	if (type == N_UNDF || type == N_PBUD)
		c = array[index->count[i]].n_value ? 'C' : 'U';
	else if (type == N_ABS)
		c = 'A';
	else if (type == N_SECT)
		c = find_seg_sym(array[index->count[i]].n_sect, arr);
	else if (type == N_INDR)
		c = 'I';
	if (c != 'U')
		put_hexa(array[index->count[i]].n_value);
	else
		ft_putstr("                ");
	if (!(array[index->count[i]].n_type & N_EXT))
		c += 32;
	if (array[index->count[i]].n_type & N_STAB)
		c = 'w';
	ft_putchar(' ');
	ft_putchar(c);
	ft_putchar(' ');
}

void		find_seg64(struct segment_command_64 *seg, char **arr)
{
	unsigned int		i;
	int					j;
	struct section_64	*sec;

	sec = (void*)seg + sizeof(struct segment_command_64);
	i = -1;
	while (arr[++i])
		;
	j = i;
	i = -1;
	while (++i < seg->nsects)
	{
		arr[j++] = sec->sectname;
		++sec;
	}
	arr[j] = NULL;
}

void		print_output(t_sorted *index, struct nlist_64 *array, \
				char *string_table, char **arr)
{
	int		i;
	int		max;

	max = index->tab_size;
	i = -1;
	while (++i <= max)
	{
		print_output2(index, array, i, arr);
		ft_putendl(string_table + array[index->count[i]].n_un.n_strx);
	}
}
