/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_otool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 14:27:02 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 15:06:59 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static char		handle_binary_otool2(void *ptr, char *name, size_t size)
{
	if (*(unsigned int*)ptr == MH_MAGIC_64 || *(unsigned int*)ptr == MH_MAGIC)
	{
		ft_putstr(name);
		ft_putendl(":");
		manage_otool(ptr, size, name);
	}
	else if ((unsigned int)*(int*)ptr == FAT_MAGIC || \
			(unsigned int)*(int*)ptr == FAT_CIGAM)
		ft_otool_uni(ptr, size, name);
	else if (*(unsigned long*)ptr == 0x0A3E686372613C21)
		manage_library_otool(ptr, size, name);
	else
		return (0);
	return (1);
}

static void		handle_binary_otool(int fd, char *name, char fail)
{
	void			*ptr;
	struct stat		buff;

	if (fstat(fd, &buff) < 0 && (fail = 1))
		ft_putstr("ft_otool: Open file failed");
	else if ((unsigned long)buff.st_size <= sizeof(struct mach_header_64) + \
		sizeof(struct load_command) && (fail = 1))
		ft_putstr("ft_otool: Binary too small\n");
	else if (S_ISDIR(buff.st_mode) && (fail = 1))
		ft_putstr("ft_otool: Is a directory\n");
	else if (!S_ISREG(buff.st_mode) && (fail = 1))
		ft_putstr("ft_otool: Is not a regular file\n");
	else if ((ptr = mmap(NULL, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) \
		< 0 && (fail = 1))
		ft_putstr("ft_otool: Mmap binary failed");
	if (fail)
		return ;
	if (!handle_binary_otool2(ptr, name, buff.st_size))
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
		handle_binary_otool(fd, av[i], 0);
		close(fd);
	}
	if (ac < 2)
		ft_fputendl("ft_otool: No such file or directory.", 2);
}
