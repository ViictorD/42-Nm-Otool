/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 22:23:18 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 17:09:30 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char		find_seg_sym(int nb, char **arr)
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

void		find_seg64(struct segment_command_64 *seg, char **arr, \
				unsigned int filesize)
{
	unsigned int		i;
	int					j;
	struct section_64	*sec;

	if (seg->filesize + seg->fileoff > filesize)
		ft_exiterror("Binary corrupted.", 1);
	sec = (void*)seg + sizeof(struct segment_command_64);
	i = -1;
	while (arr[++i])
		;
	j = i;
	i = -1;
	while (++i < swap_bits(seg->nsects, 32))
	{
		arr[j++] = sec->sectname;
		++sec;
	}
	arr[j] = NULL;
}

void		print_output(t_block *begin)
{
	t_block	*save;

	while (begin)
	{
		if (begin->next && !ft_strcmp(begin->name, begin->next->name) && \
			begin->sym > 64 && begin->sym < 91 && \
			begin->next->sym > begin->sym)
		{
			save = begin;
			begin = begin->next;
			save->next = begin->next;
			begin->next = save;
			continue ;
		}
		if (begin->addr != 0 || begin->sym == 'A' || (begin->sym == 'T') || \
			(begin->sym == 't'))
			put_hexa(begin->addr, 16);
		else
			ft_putstr("                ");
		ft_putchar(' ');
		ft_putchar(begin->sym);
		ft_putchar(' ');
		ft_putendl(begin->name);
		begin = begin->next;
	}
}

char		get_sym_char(struct nlist_64 nlist, char **sectname)
{
	char	type;
	char	c;

	c = 'U';
	type = nlist.n_type & N_TYPE;
	if (type == N_UNDF || type == N_PBUD)
		c = swap_bits(nlist.n_value, 64) ? 'C' : 'U';
	else if (type == N_ABS)
		c = 'A';
	else if (type == N_SECT)
		c = find_seg_sym(nlist.n_sect, sectname);
	else if (type == N_INDR)
		c = 'I';
	if (!(nlist.n_type & N_EXT))
		c += 32;
	if (nlist.n_type & N_STAB)
		c = 'w';
	return (c);
}
