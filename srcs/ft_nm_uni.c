/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_uni.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 21:18:40 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 15:37:26 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char		manage_nm(void *ptr, unsigned int filesize, char *name)
{
	if (*(unsigned int*)ptr == MH_CIGAM_64 || *(unsigned int*)ptr == MH_CIGAM \
		|| *(unsigned int*)ptr == FAT_CIGAM)
		get_rev(1, 1);
	else
		get_rev(1, 0);
	if (*(unsigned int*)ptr == MH_CIGAM_64 || \
		*(unsigned int*)ptr == MH_MAGIC_64)
		ft_nm(ptr, filesize, ptr, ptr + sizeof(struct mach_header_64));
	else if (*(unsigned int*)ptr == MH_CIGAM || *(unsigned int*)ptr == MH_MAGIC)
		ft_nm_32(ptr, filesize, ptr, ptr + sizeof(struct mach_header));
	else if (*(unsigned long*)ptr == 0x0A3E686372613C21)
		manage_library(ptr, filesize, name);
	else
		return (0);
	return (1);
}

static void	try_all_arch_nm(void *ptr, unsigned int filesize, unsigned i, \
				char *name)
{
	struct fat_arch		*arch;
	void				*new_ptr;
	unsigned int		nb;
	const NXArchInfo	*cpu;

	nb = uswap_32(((struct fat_header*)ptr)->nfat_arch);
	while (i < nb)
	{
		arch = (struct fat_arch*)(ptr + sizeof(struct fat_header) + \
			(sizeof(struct fat_arch) * i++));
		new_ptr = ptr + uswap_32(arch->offset);
		cpu = NXGetArchInfoFromCpuType(swap_32(arch->cputype), \
			swap_32(arch->cpusubtype));
		ft_putchar('\n');
		ft_putstr(name);
		ft_putstr(" (for architecture ");
		ft_putstr(cpu->name);
		ft_putstr("):\n");
		if (!manage_nm(new_ptr, filesize, name))
		{
			ft_fputstr("ft_nm: The file was not recognized as a valid ", 2);
			ft_fputstr("object file\n", 2);
		}
	}
}

static char	go_x86_64_nm(void *ptr, struct fat_arch *arch, \
				unsigned int filesize, char *name)
{
	void	*new_ptr;

	if (*(unsigned int*)ptr == FAT_MAGIC_64 || \
		*(unsigned int*)ptr == FAT_CIGAM_64)
	{
		if (uswap_64(arch->offset) >= filesize)
			ft_exiterror("Binary corrupted", 1);
		new_ptr = ptr + uswap_64(arch->offset);
	}
	else
	{
		if (uswap_32(arch->offset) >= filesize)
			ft_exiterror("Binary corrupted", 1);
		new_ptr = ptr + uswap_32(arch->offset);
	}
	if (*(unsigned long*)new_ptr == 0x0A3E686372613C21)
		filesize = uswap_32(arch->size);
	if (!manage_nm(new_ptr, filesize, name))
		return (1);
	return (0);
}

static void	print_name(char *name)
{
	ft_putchar('\n');
	ft_putstr(name);
	ft_putendl(":");
}

void		ft_nm_uni(void *ptr, unsigned int filesize, char *name, int j)
{
	struct fat_arch		*arch;
	unsigned int		nb;
	unsigned int		i;
	const NXArchInfo	*cpu;

	nb = uswap_32(((struct fat_header*)ptr)->nfat_arch);
	i = 0;
	while (i < nb)
	{
		arch = (struct fat_arch*)(ptr + sizeof(struct fat_header) + \
			(sizeof(struct fat_arch) * i++));
		cpu = NXGetArchInfoFromCpuType(swap_32(arch->cputype), \
			swap_32(arch->cpusubtype));
		if (cpu && !ft_strcmp("x86_64", cpu->name))
		{
			if (j > 3)
				print_name(name);
			if (go_x86_64_nm(ptr, arch, filesize, name))
				break ;
			return ;
		}
	}
	try_all_arch_nm(ptr, filesize, 0, name);
}
