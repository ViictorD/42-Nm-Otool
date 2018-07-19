/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_uni.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 17:31:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/19 20:01:20 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

char	is_ppc_arch(char set, char value)
{
	static	char	ppc = 0;
	if (set)
		ppc = value;
	return (ppc);

}

char	manage_otool(void *ptr, unsigned int filesize, char *name)
{
	if (*(unsigned int*)ptr == MH_CIGAM_64 || *(unsigned int*)ptr == MH_CIGAM \
		|| *(unsigned int*)ptr == FAT_CIGAM)
		get_rev(1, 1);
	else
		get_rev(1, 0);
	if (*(unsigned int*)ptr == MH_CIGAM_64 || *(unsigned int*)ptr == MH_MAGIC_64)
		ft_otool(ptr, ptr, 0, filesize);
	else if (*(unsigned int*)ptr == MH_CIGAM || *(unsigned int*)ptr == MH_MAGIC)
		ft_otool_32(ptr, ptr, 0, filesize);
	else if (*(unsigned long*)ptr == 0x0A3E686372613C21)
		manage_library_otool(ptr, filesize, name);
	else
		return (0);
	(void)name;
	return (1);
}

void	ft_otool_uni(void *ptr, unsigned int filesize, char *name)
{
	void				*new_ptr;
	struct fat_arch		*arch;
	unsigned int 		nb;
	unsigned int		i;
	const NXArchInfo	*cpu;

	nb = uswap_32(((struct fat_header*)ptr)->nfat_arch);
	i = 0;
	while (i < nb)
	{
		arch = (struct fat_arch*)(ptr + sizeof(struct fat_header) + \
			(sizeof(struct fat_arch) * i));
		cpu = NXGetArchInfoFromCpuType(swap_32(arch->cputype), swap_32(arch->cpusubtype));
		if (cpu && !ft_strcmp("x86_64", cpu->name))
		{
			if (*(unsigned int*)ptr == FAT_MAGIC_64 || *(unsigned int*)ptr == FAT_CIGAM_64)
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
				break ;
			return ;
		}
		++i;
	}
	i = 0;
	while (i < nb)
	{
		arch = (struct fat_arch*)(ptr + sizeof(struct fat_header) + \
			(sizeof(struct fat_arch) * i));
		new_ptr = ptr + uswap_32(arch->offset);
		cpu = NXGetArchInfoFromCpuType(swap_32(arch->cputype), swap_32(arch->cpusubtype));
		ft_putstr(name);
		ft_putstr(" (architecture ");
		ft_putstr(cpu->name);
		ft_putstr("):\n");
		if (!ft_strcmp(cpu->name, "ppc"))
			is_ppc_arch(1, 1);
		if (!manage_otool(new_ptr, filesize, name))
		{
			ft_fputstr(name, 2);
			ft_fputstr(": is not an object file\n", 2);
		}
		is_ppc_arch(1, 0);
		++i;
	}
}