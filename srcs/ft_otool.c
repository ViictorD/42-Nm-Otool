/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 22:47:58 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/06/27 17:02:05 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void		print_one_hexa(unsigned char nb, char first)
{
	if (first && (nb < 16 || !nb))
		ft_putchar('0');
	if (nb >= 16)
		print_one_hexa(nb / 16, 0);
	if (nb % 16 < 10)
		ft_putchar((nb % 16) + 48);
	else
		ft_putchar((nb % 16) + 87);
}

static void		print_otool(struct section_64 *sec, void *ptr)
{
	int				i;
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

static void		ft_otool(void *ptr, struct mach_header_64 *header, \
					struct load_command *lc, int i)
{
	struct segment_command_64	*seg;
	struct section_64			*sec;
	int							j;

	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64*)lc;
			sec = (struct section_64*)((void*)lc + sizeof(*seg));
			if (CHECK_SECTION(sec->segname, sec->sectname))
			{
				j = -1;
				while (++j < seg->nsects)
				{
					sec = (struct section_64*)((void*)lc + sizeof(*seg) + \
						(sizeof(struct section_64) * j));
					if (CHECK_SECTION(sec->segname, sec->sectname))
						print_otool(sec, ptr + sec->offset);
				}
			}
		}
		lc = (void*)lc + lc->cmdsize;
		++i;
	}
}

static void		handle_binary_otool(int fd)
{
	void			*ptr;
	struct stat		buff;

	if (fstat(fd, &buff) < 0)
	{
		ft_putstr("ft_otool: Open file failed");
		return ;
	}
	if ((ptr = mmap(NULL, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) < 0)
	{
		ft_putstr("ft_otool: Mmap binary failed");
		return ;
	}
	if ((unsigned int)*(int*)ptr == MH_MAGIC_64)
		ft_otool(ptr, (struct mach_header_64*)ptr, ptr + sizeof(struct mach_header_64), 0);
	if ((munmap(ptr, buff.st_size)) < 0)
	{
		ft_putstr("ft_otool: Munmap binary failed");
		return ;
	}
}

int				main(int ac, char **av)
{
	int		i;
	int		fd;

	if (ac < 2)
	{
		ft_fputstr("ft_otool: No such file or directory.", 2);
		return (1);
	}
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
}
