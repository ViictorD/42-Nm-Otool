/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_32.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 17:57:26 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 14:32:09 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		print_otool_32(struct section *sec, void *ptr)
{
	unsigned int	i;
	int				j;
	unsigned char	tmp;

	i = 0;
	ft_putendl("Contents of (__TEXT,__text) section");
	while (i < swap_bits(sec->size, 32))
	{
		put_hexa((long)(swap_bits(sec->addr, 32) + i), 8);
		ft_putchar('\t');
		j = -1;
		while (++j < 16 && i + j < swap_bits(sec->size, 32))
		{
			tmp = *(unsigned char*)(ptr + i + j);
			print_one_hexa(tmp, 1);
			if (!is_ppc_arch(0, 0) || \
				(is_ppc_arch(0, 0) && (j + 1) % 4 == 0 && j))
				ft_putchar(' ');
		}
		ft_putchar('\n');
		i += 16;
	}
}

static void		ft_otool_322(void *ptr, struct load_command *lc, \
					struct segment_command *seg, size_t filesize)
{
	unsigned int	j;
	struct section	*sec;

	j = -1;
	while (++j < swap_bits(seg->nsects, 32))
	{
		sec = (struct section*)((void*)lc + sizeof(*seg) + \
			(sizeof(struct section) * j));
		if (swap_bits(sec->offset, 32) + swap_bits(sec->size, 32) > filesize)
			ft_exiterror("Binary corrupted", 1);
		if (C_SECTION(sec->segname, sec->sectname))
			print_otool_32(sec, ptr + swap_bits(sec->offset, 32));
	}
}

void			ft_otool_32(void *ptr, struct mach_header *header, \
					unsigned int i, size_t filesize)
{
	struct segment_command	*seg;
	struct section			*sec;
	struct load_command		*lc;

	lc = ptr + sizeof(struct mach_header);
	while (i < swap_bits(header->ncmds, 32))
	{
		if (swap_bits(lc->cmd, 32) == LC_SEGMENT)
		{
			seg = (struct segment_command*)lc;
			sec = (struct section*)((void*)lc + sizeof(*seg));
			if (C_SECTION(sec->segname, sec->sectname))
				ft_otool_322(ptr, lc, seg, filesize);
		}
		lc = (void*)lc + swap_bits(lc->cmdsize, 32);
		++i;
	}
}
