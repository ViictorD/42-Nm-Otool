/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 22:48:43 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 15:56:46 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/arch.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "../libft/libft.h"

# define HEAD_OBJ_SIZE	0x30
# define HEAD_OBJ_NAME	0x3C
# define HEAD_OBJ_END	0x3C
# define C_SECTION(x, y)	!ft_strcmp("__TEXT", x) && !ft_strcmp("__text", y)

void				put_hexa(long nb, int size);
void				print_one_hexa(unsigned char nb, char first);
void				check_corrupted(struct nlist_64 *array, \
						unsigned int nsyms, unsigned int filesize);
void				check_corrupted_32(struct nlist *array, \
						unsigned int nsyms, unsigned int filesize);
unsigned long		swap_bits(unsigned long value, int base);
unsigned int		uswap_32(unsigned int x);
unsigned long		uswap_64(unsigned long x);
int					swap_32(int x);
char				manage_otool(void *ptr, unsigned int filesize, char *name);
void				ft_otool_32(void *ptr, struct mach_header *header, \
						unsigned int i, size_t filesize);
void				ft_otool(void *ptr, struct mach_header_64 *header, \
						unsigned int i, size_t filesize);
char				get_rev(char set, char value);
void				ft_otool_uni(void *ptr, unsigned int filesize, char *name);
char				is_ppc_arch(char set, char value);
void				manage_library_otool(void *ptr, unsigned int filesize, \
						char *name);

#endif
