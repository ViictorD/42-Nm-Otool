/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 21:58:29 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 17:01:23 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_block	*manage_and_sort2(t_block **begin, char *string_table, \
				struct nlist_64 *array, int i)
{
	t_block			*sort;
	t_block			*last;

	sort = *begin;
	last = sort;
	while (sort)
	{
		if (ft_strcmp(string_table + swap_bits(array[i].n_un.n_strx, 32), \
			sort->name) < 0)
		{
			if (include_before(begin, sort, last, \
				new_block(i, array[i], string_table)) == *begin)
				return (*begin);
			else
				return (last->next);
		}
		else if (!sort->next)
		{
			sort->next = new_block(i, array[i], string_table);
			return (sort->next);
		}
		last = sort;
		sort = sort->next;
	}
	return (NULL);
}

static t_block	*manage_and_sort(struct nlist_64 *array, unsigned int nsyms, \
				char *string_table, char **sectname)
{
	unsigned int	i;
	t_block			*begin;
	char			type;
	t_block			*tmp;

	begin = NULL;
	i = 0;
	while (i < nsyms)
	{
		type = array[i].n_type & N_TYPE;
		if ((type == N_UNDF || type == N_PBUD || type == N_ABS || \
			type == N_SECT || type == N_INDR) && !(array[i].n_type & N_STAB))
		{
			tmp = manage_and_sort2(&begin, string_table, array, i);
			if (!begin)
			{
				begin = new_block(i, array[i], string_table);
				begin->sym = get_sym_char(array[i], sectname);
			}
			else if (tmp)
				tmp->sym = get_sym_char(array[i], sectname);
		}
		++i;
	}
	return (begin);
}

static t_block	*end_nm(void *ptr, struct symtab_command *sym, \
					unsigned int filesize, char **arr)
{
	check_corrupted(ptr + swap_bits(sym->symoff, 32), \
		swap_bits(sym->nsyms, 32), filesize);
	return (manage_and_sort(ptr + swap_bits(sym->symoff, 32), \
		swap_bits(sym->nsyms, 32), ptr + swap_bits(sym->stroff, 32), arr));
}

void			ft_nm(void *ptr, unsigned int filesize, \
					struct mach_header_64 *header, struct load_command *lc)
{
	struct symtab_command	*sym;
	unsigned int			i;
	t_block					*sorted;
	char					*arr[256];

	i = 0;
	arr[0] = NULL;
	while (i++ < swap_bits(header->ncmds, 32))
	{
		if (swap_bits(lc->cmd, 32) == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			if (swap_bits(sym->symoff, 32) > filesize)
				ft_exiterror("Binary corrupted", 1);
		}
		else if (swap_bits(lc->cmd, 32) == LC_SEGMENT_64)
			find_seg64((struct segment_command_64*)lc, arr, filesize);
		lc = (void*)lc + swap_bits(lc->cmdsize, 32);
		if ((void*)lc - ptr >= filesize)
			ft_exiterror("Binary corrupted", 1);
	}
	sorted = end_nm(ptr, sym, filesize, arr);
	print_output(sorted);
	free_block(sorted);
}
