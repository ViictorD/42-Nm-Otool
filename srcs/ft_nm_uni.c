/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_uni.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 21:18:40 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/17 21:57:13 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	manage_nm(void *ptr, unsigned int filesize)
{
	if (*(unsigned int*)ptr == MH_CIGAM_64 || *(unsigned int*)ptr == MH_CIGAM \
		|| *(unsigned int*)ptr == FAT_CIGAM)
		get_rev(1, 1);
	else
		get_rev(1, 0);
	if (*(unsigned int*)ptr == MH_CIGAM_64 || *(unsigned int*)ptr == MH_MAGIC_64)
		ft_nm(ptr, filesize, ptr, ptr + sizeof(struct mach_header_64));
	else if (*(unsigned int*)ptr == MH_CIGAM || *(unsigned int*)ptr == MH_MAGIC)
		ft_nm_32(ptr, filesize, ptr, ptr + sizeof(struct mach_header));
	else
		return (0);
	return (1);
}

void	ft_nm_uni(void *ptr, unsigned int filesize, char *name, int j)
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
			if (j > 3)
			{
				ft_putchar('\n');
				ft_putstr(name);
				ft_putendl(":");
			}
			if (*(unsigned int*)ptr == FAT_MAGIC_64 || *(unsigned int*)ptr == FAT_CIGAM_64)
				new_ptr = ptr + uswap_64(arch->offset);
			else
				new_ptr = ptr + uswap_32(arch->offset);
			manage_nm(new_ptr, filesize);
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
		ft_putchar('\n');
		ft_putstr(name);
		ft_putstr(" (for architecture ");
		ft_putstr(cpu->name);
		ft_putstr("):\n");
		if (!manage_nm(new_ptr, filesize))
			ft_fputstr("ft_nm: The file was not recognized as a valid object file\n", 2);
		++i;
	}
}