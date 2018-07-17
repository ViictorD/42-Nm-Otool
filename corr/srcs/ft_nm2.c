/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 22:23:18 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/17 20:07:54 by vdarmaya         ###   ########.fr       */
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

void		print_output(t_block *begin)
{
	while (begin)
	{
		if (begin->addr != 0)
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
		c = nlist.n_value ? 'C' : 'U';
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
