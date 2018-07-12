/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 21:58:29 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/12 15:58:37 by vdarmaya         ###   ########.fr       */
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
		if (ft_strcmp(string_table + array[i].n_un.n_strx, sort->name) < 0)
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

static void		ft_nm(void *ptr, unsigned int filesize, \
			struct mach_header_64 *header, struct load_command *lc)
{
	struct symtab_command	*sym;
	unsigned int			i;
	t_block					*sorted;
	char					*arr[256];

	i = 0;
	arr[0] = NULL;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			if (sym->symoff > filesize)
				ft_exiterror("Binary corrupted", 1);
		}
		else if (lc->cmd == LC_SEGMENT_64)
			find_seg64((struct segment_command_64*)lc, arr);
		lc = (void*)lc + lc->cmdsize;
		++i;
	}
	sorted = manage_and_sort(ptr + sym->symoff, sym->nsyms, \
		ptr + sym->stroff, arr);
	print_output(sorted);
	free_block(sorted);
}

static void		handle_binary(int fd)
{
	void			*ptr;
	struct stat		buff;
	char			fail;

	fail = 0;
	if (fstat(fd, &buff) < 0 && (fail = 1))
		ft_putstr("ft_nm: Open file failed\n");
	else if ((unsigned long)buff.st_size <= sizeof(struct mach_header_64) + \
		sizeof(struct load_command) && (fail = 1))
		ft_putstr("ft_nm: Binary too small\n");
	else if (S_ISDIR(buff.st_mode) && (fail = 1))
		ft_putstr("ft_nm: Is a directory\n");
	else if (!S_ISREG(buff.st_mode) && (fail = 1))
		ft_putstr("ft_nm: Is not a regular file\n");
	else if ((ptr = mmap(NULL, buff.st_size, PROT_READ, MAP_PRIVATE, fd, \
		0)) < 0 && (fail = 1))
		ft_putstr("ft_nm: Mmap binary failed");
	if (fail)
		return ;
	if ((unsigned int)*(int*)ptr == MH_MAGIC_64)
		ft_nm(ptr, buff.st_size, ptr, ptr + sizeof(struct mach_header_64));
	else
		ft_fputstr("ft_nm: Not a 64 bits file format\n", 2);
	if ((munmap(ptr, buff.st_size)) < 0)
		ft_putstr("ft_nm: Munmap binary failed");
}

int				main(int ac, char **av)
{
	int		i;
	int		fd;

	i = 0;
	while (++i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) < 0)
		{
			ft_fputstr("ft_nm: ", 2);
			ft_fputstr(av[i], 2);
			ft_fputendl(": No such file or directory.", 2);
			continue ;
		}
		if (ac > 2)
		{
			ft_putchar('\n');
			ft_putstr(av[i]);
			ft_putendl(":");
		}
		handle_binary(fd);
		close(fd);
	}
	if (ac < 2)
		ft_fputendl("ft_nm: No such file or directory.", 2);
	return (0);
}
