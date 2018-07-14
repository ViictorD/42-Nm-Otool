/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 21:59:37 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/07 02:43:47 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/stab.h>
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
void				put_hexa(long nb);
char				get_sym_char(struct nlist_64 nlist, char **sectname);

#endif
