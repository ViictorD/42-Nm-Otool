/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_uni.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 17:31:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 17:27:33 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

char		manage_otool(void *ptr, unsigned int filesize, char *name)
{
	if (*(unsigned int*)ptr == MH_CIGAM_64 || *(unsigned int*)ptr == MH_CIGAM \
		|| *(unsigned int*)ptr == FAT_CIGAM)
		get_rev(1, 1);
	else
		get_rev(1, 0);
	if (*(unsigned int*)ptr == MH_CIGAM_64 || \
		*(unsigned int*)ptr == MH_MAGIC_64)
		ft_otool(ptr, ptr, 0, filesize);
	else if (*(unsigned int*)ptr == MH_CIGAM || *(unsigned int*)ptr == MH_MAGIC)
		ft_otool_32(ptr, ptr, 0, filesize);
	else if (*(unsigned long*)ptr == 0x0A3E686372613C21)
		manage_library_otool(ptr, filesize, name);
	else
		return (0);
	return (1);
}

static void	print_name_arch(char *name, const char *cpu)
{
	ft_putstr(name);
	ft_putstr(" (architecture ");
	ft_putstr(cpu);
	ft_putstr("):\n");
}

static void	try_all_arch_otool(void *ptr, unsigned int filesize, \
				unsigned int i, char *name)
{
	unsigned int		nb;
	void				*new_ptr;
	const NXArchInfo	*cpu;
	struct fat_arch		*arch;

	nb = uswap_32(((struct fat_header*)ptr)->nfat_arch);
	while (i < nb)
	{
		arch = (struct fat_arch*)(ptr + sizeof(struct fat_header) + \
			(sizeof(struct fat_arch) * i++));
		new_ptr = ptr + uswap_32(arch->offset);
		if ((long)new_ptr >= (long)(ptr + filesize))
			ft_exiterror("Binary corrupted.", 1);
		cpu = NXGetArchInfoFromCpuType(swap_32(arch->cputype), \
			swap_32(arch->cpusubtype));
		print_name_arch(name, cpu->name);
		!ft_strcmp(cpu->name, "ppc") ? is_ppc_arch(1, 1) : 0;
		if (!manage_otool(new_ptr, filesize, name))
		{
			ft_fputstr(name, 2);
			ft_fputstr(": is not an object file\n", 2);
		}
		is_ppc_arch(1, 0);
	}
}

static char	go_x86_64(void *ptr, struct fat_arch *arch, unsigned int filesize, \
				char *name)
{
	void				*new_ptr;

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
	if (*(unsigned long*)new_ptr != 0x0A3E686372613C21)
	{
		ft_putstr(name);
		ft_putendl(":");
	}
	if (*(unsigned long*)new_ptr == 0x0A3E686372613C21)
		filesize = uswap_32(arch->size);
	if (!manage_otool(new_ptr, filesize, name))
		return (1);
	return (0);
}

void		ft_otool_uni(void *ptr, unsigned int filesize, char *name)
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
			(sizeof(struct fat_arch) * i));
		cpu = NXGetArchInfoFromCpuType(swap_32(arch->cputype), \
			swap_32(arch->cpusubtype));
		if (cpu && !ft_strcmp("x86_64", cpu->name))
		{
			if (go_x86_64(ptr, arch, filesize, name))
				break ;
			return ;
		}
		++i;
	}
	try_all_arch_otool(ptr, filesize, 0, name);
}
