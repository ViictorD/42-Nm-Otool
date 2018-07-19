/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 22:47:58 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/19 19:49:38 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		print_otool(struct section_64 *sec, void *ptr)
{
	unsigned int	i;
	int				j;
	unsigned char	tmp;

	i = 0;
	ft_putendl("Contents of (__TEXT,__text) section");
	while (i < swap_bits(sec->size, 64))
	{
		put_hexa((long)(swap_bits(sec->addr, 64) + i), 16);
		ft_putchar('\t');
		j = -1;
		while (++j < 16 && i + j < swap_bits(sec->size, 64))
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

static void		ft_otool2(void *ptr, struct load_command *lc, \
					struct segment_command_64 *seg, size_t filesize)
{
	unsigned int		j;
	struct section_64	*sec;

	j = -1;
	while (++j < swap_bits(seg->nsects, 32))
	{
		sec = (struct section_64*)((void*)lc + sizeof(*seg) + \
			(sizeof(struct section_64) * j));
		if (swap_bits(sec->offset, 32) + swap_bits(sec->size, 64) > filesize)
			ft_exiterror("Binary corrupted", 1);
		if (C_SECTION(sec->segname, sec->sectname))
			print_otool(sec, ptr + swap_bits(sec->offset,32));
	}
}

void			ft_otool(void *ptr, struct mach_header_64 *header, \
					unsigned int i, size_t filesize)
{
	struct segment_command_64	*seg;
	struct section_64			*sec;
	struct load_command			*lc;

	lc = ptr + sizeof(struct mach_header_64);
	while (i < header->ncmds)
	{
		if (swap_bits(lc->cmd, 32) == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64*)lc;
			sec = (struct section_64*)((void*)lc + sizeof(*seg));
			if (C_SECTION(sec->segname, sec->sectname))
				ft_otool2(ptr, lc, seg, filesize);
		}
		lc = (void*)lc + lc->cmdsize;
		++i;
	}
}

static void		handle_binary_otool(int fd, char *name)
{
	void			*ptr;
	struct stat		buff;
	char			fail;

	fail = 0;
	if (fstat(fd, &buff) < 0 && (fail = 1))
		ft_putstr("ft_otool: Open file failed");
	else if ((unsigned long)buff.st_size <= sizeof(struct mach_header_64) + \
		sizeof(struct load_command) && (fail = 1))
		ft_putstr("ft_otool: Binary too small\n");
	else if (S_ISDIR(buff.st_mode) && (fail = 1))
		ft_putstr("ft_otool: Is a directory\n");
	else if (!S_ISREG(buff.st_mode) && (fail = 1))
		ft_putstr("ft_otool: Is not a regular file\n");
	else if ((ptr = mmap(NULL, buff.st_size, PROT_READ, MAP_PRIVATE, fd, \
		0)) < 0 && (fail = 1))
		ft_putstr("ft_otool: Mmap binary failed");
	if (fail)
		return ;
	if (*(unsigned int*)ptr == MH_MAGIC_64 || *(unsigned int*)ptr == MH_MAGIC)
	{
		ft_putstr(name);
		ft_putendl(":");
		manage_otool(ptr, buff.st_size, name);
	}
	else if ((unsigned int)*(int*)ptr == FAT_MAGIC || (unsigned int)*(int*)ptr == FAT_CIGAM)
		ft_otool_uni(ptr, buff.st_size, name);
	else if (*(unsigned long*)ptr == 0x0A3E686372613C21)
		manage_library_otool(ptr, buff.st_size, name);
	else
	{
		ft_fputstr(name, 2);
		ft_fputstr(": is not an object file\n", 2);
	}
	if ((munmap(ptr, buff.st_size)) < 0)
		ft_putstr("ft_otool: Munmap binary failed");
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
			ft_fputstr("ft_otool: ", 2);
			ft_fputstr(av[i], 2);
			ft_fputendl(": No such file or directory.", 2);
			continue ;
		}
		// ft_putstr(av[i]);
		// ft_putendl(":");
		handle_binary_otool(fd, av[i]);
		close(fd);
	}
	if (ac < 2)
		ft_fputendl("ft_otool: No such file or directory.", 2);
}
