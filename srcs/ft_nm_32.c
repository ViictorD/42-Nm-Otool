#include "ft_nm.h"

static t_block	*manage_and_sort_322(t_block **begin, char *string_table, \
				struct nlist *array, int i)
{
	t_block			*sort;
	t_block			*last;

	sort = *begin;
	last = sort;
	while (sort)
	{
		if (ft_strcmp(string_table + swap_bits(array[i].n_un.n_strx, 32), sort->name) < 0) // maybe prb
		{
			if (include_before(begin, sort, last, \
				new_block_32(i, array[i], string_table)) == *begin)
				return (*begin);
			else
				return (last->next);
		}
		else if (!sort->next)
		{
			sort->next = new_block_32(i, array[i], string_table);
			return (sort->next);
		}
		last = sort;
		sort = sort->next;
	}
	return (NULL);
}

static t_block	*manage_and_sort_32(struct nlist *array, unsigned int nsyms, \
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
			tmp = manage_and_sort_322(&begin, string_table, array, i);
			if (!begin)
			{
				begin = new_block_32(i, array[i], string_table);
				begin->sym = get_sym_char_32(array[i], sectname);
			}
			else if (tmp)
				tmp->sym = get_sym_char_32(array[i], sectname);
		}
		++i;
	}
	return (begin);
}


void		ft_nm_32(void *ptr, unsigned int filesize, \
			struct mach_header *header, struct load_command *lc)
{
	struct symtab_command	*sym;
	unsigned int			i;
	t_block					*sorted;
	char					*arr[256];

	i = 0;
	arr[0] = NULL;
	while (i < swap_bits(header->ncmds, 32))
	{
		if (swap_bits(lc->cmd, 32) == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			if (swap_bits(sym->symoff, 32) > filesize)
				ft_exiterror("Binary corrupted", 1);
		}
		else if (swap_bits(lc->cmd, 32) == LC_SEGMENT)
			find_seg32((struct segment_command*)lc, arr);
		lc = (void*)lc + swap_bits(lc->cmdsize, 32);
		++i;
	}
	sorted = manage_and_sort_32(ptr + swap_bits(sym->symoff, 32), swap_bits(sym->nsyms, 32), \
		ptr + swap_bits(sym->stroff, 32), arr);
	print_output_32(sorted);
	free_block(sorted);
}