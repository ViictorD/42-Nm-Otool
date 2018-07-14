/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 22:47:58 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/12 16:03:57 by vdarmaya         ###   ########.fr       */
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
	while (i < sec->size)
	{
		put_hexa((long)(sec->addr + i));
		ft_putchar('\t');
		j = -1;
		while (++j < 16 && i + j < sec->size)
		{
			tmp = *(unsigned char*)(ptr + i + j);
			print_one_hexa(tmp, 1);
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
	while (++j < seg->nsects)
	{
		sec = (struct section_64*)((void*)lc + sizeof(*seg) + \
			(sizeof(struct section_64) * j));
		if (sec->offset + sec->size > filesize)
			ft_exiterror("Binary corrupted", 1);
		if (C_SECTION(sec->segname, sec->sectname))
			print_otool(sec, ptr + sec->offset);
	}
}

static void		ft_otool(void *ptr, struct mach_header_64 *header, \
					unsigned int i, size_t filesize)
{
	struct segment_command_64	*seg;
	struct section_64			*sec;
	struct load_command			*lc;

	lc = ptr + sizeof(struct mach_header_64);
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
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

static void		handle_binary_otool(int fd)
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
	if ((unsigned int)*(int*)ptr == MH_MAGIC_64)
		ft_otool(ptr, (struct mach_header_64*)ptr, 0, buff.st_size);
	else
		ft_fputstr("ft_nm: Not a 64 bits file format\n", 2);
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
		ft_putstr(av[i]);
		ft_putendl(":");
		handle_binary_otool(fd);
		close(fd);
	}
	if (ac < 2)
		ft_fputendl("ft_otool: No such file or directory.", 2);
}
