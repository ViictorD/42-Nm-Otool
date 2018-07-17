/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 21:59:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/17 21:30:45 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/stab.h>
# include <mach-o/fat.h>
# include <mach-o/arch.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef struct		s_block
{
	int				index;
	char			*name;
	char			sym;
	long			addr;
	struct s_block	*next;
}					t_block;

typedef struct		s_sorted
{
	int				*count;
	int				tab_size;
}					t_sorted;

t_block				*include_before(t_block **begin, t_block *sort, \
						t_block *last, t_block *new);
t_block				*new_block(int i, struct nlist_64 nlist, \
						char *string_table);
void				free_block(t_block *b);
void				print_output(t_block *begin);
void				find_seg64(struct segment_command_64 *seg, char **arr);
void				put_hexa(long nb, int size);
char				get_sym_char(struct nlist_64 nlist, char **sectname);
void				ft_nm_uni(void *ptr, unsigned int filesize, char *name, int j);
void				ft_nm(void *ptr, unsigned int filesize, \
						struct mach_header_64 *header, struct load_command *lc);
void				ft_nm_32(void *ptr, unsigned int filesize, \
						struct mach_header *header, struct load_command *lc);
unsigned long		swap_bits(unsigned long value, int base);
int					swap_32(int x);
unsigned int		uswap_32(unsigned int x);
unsigned long		uswap_64(unsigned long x);
char				get_rev(char set, char value);
t_block				*new_block_32(int i, struct nlist nlist, char *string_table);
char				get_sym_char_32(struct nlist nlist, char **sectname);
void				find_seg32(struct segment_command *seg, char **arr);
char				find_seg_sym(int nb, char **arr);
char				manage_nm(void *ptr, unsigned int filesize);
void				print_output_32(t_block *begin);


#endif
