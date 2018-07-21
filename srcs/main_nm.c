/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_nm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 15:38:07 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 15:41:54 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		handle_binary2(void *ptr, char *name, unsigned int size, int i)
{
	if (*(unsigned int*)ptr == MH_MAGIC_64 || *(unsigned int*)ptr == MH_MAGIC)
	{
		if (i > 2)
		{
			ft_putchar('\n');
			ft_putstr(name);
			ft_putendl(":");
		}
		manage_nm(ptr, size, name);
	}
	else if ((unsigned int)*(int*)ptr == FAT_MAGIC || \
		(unsigned int)*(int*)ptr == FAT_CIGAM)
		ft_nm_uni(ptr, size, name, i);
	else if (*(unsigned long*)ptr == 0x0A3E686372613C21)
		manage_library(ptr, size, name);
	else
	{
		ft_fputstr("ft_nm: The file was not recognized ", 2);
		ft_fputstr("as a valid object file\n", 2);
	}
}

static void		handle_binary(int fd, char *name, int i)
{
	void			*ptr;
	struct stat		buff;
	char			fail;

	fail = 0;
	if (fstat(fd, &buff) < 0 && (fail = 1))
		ft_fputstr("ft_nm: Open file failed\n", 2);
	else if ((unsigned long)buff.st_size <= sizeof(struct mach_header_64) + \
		sizeof(struct load_command) && (fail = 1))
	{
		ft_fputstr("ft_nm: ft_nm: The file was not recognized ", 2);
		ft_fputstr("as a valid object file\n", 2);
	}
	else if (S_ISDIR(buff.st_mode) && (fail = 1))
		ft_fputstr("ft_nm: Is a directory\n", 2);
	else if (!S_ISREG(buff.st_mode) && (fail = 1))
		ft_fputstr("ft_nm: Is not a regular file\n", 2);
	else if ((ptr = mmap(NULL, buff.st_size, PROT_READ, MAP_PRIVATE, fd, \
		0)) < 0 && (fail = 1))
		ft_fputstr("ft_nm: Mmap failed", 2);
	if (fail)
		return ;
	handle_binary2(ptr, name, buff.st_size, i);
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
		handle_binary(fd, av[i], i);
		close(fd);
	}
	if (ac < 2)
		ft_fputendl("ft_nm: No such file or directory.", 2);
	return (0);
}
