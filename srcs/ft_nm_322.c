/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_322.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 19:26:20 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/19 11:33:15 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char		get_sym_char_32(struct nlist nlist, char **sectname)
{
	char	type;
	char	c;

	c = 'U';
	type = nlist.n_type & N_TYPE;
	if (type == N_UNDF || type == N_PBUD)
		c = swap_bits(nlist.n_value, 32) ? 'C' : 'U';
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

void		find_seg32(struct segment_command *seg, char **arr)
{
	unsigned int		i;
	int					j;
	struct section	*sec;

	sec = (void*)seg + sizeof(struct segment_command);
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

void		print_output_32(t_block *begin)
{
	while (begin)
	{
		if (begin->addr != 0 || begin->sym == 't')
			put_hexa(begin->addr, 8);
		else
			ft_putstr("        ");
		ft_putchar(' ');
		ft_putchar(begin->sym);
		ft_putchar(' ');
		ft_putendl(begin->name);
		begin = begin->next;
	}
}