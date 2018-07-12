/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 20:12:15 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/07 02:43:32 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		free_block(t_block *b)
{
	t_block	*tmp;

	while (b)
	{
		tmp = b->next;
		free(b->name);
		free(b);
		b = tmp;
	}
}

t_block		*new_block(int i, struct nlist_64 nlist, char *string_table)
{
	t_block	*new;

	if (!(new = (t_block*)malloc(sizeof(t_block))))
		ft_exiterror("Malloc failed\n", 1);
	new->index = i;
	new->sym = 0;
	if (new->sym != 'U')
		new->addr = nlist.n_value;
	else
		new->addr = 0;
	new->name = ft_strdup(string_table + nlist.n_un.n_strx);
	new->next = NULL;
	return (new);
}

t_block		*include_before(t_block **begin, t_block *sort, t_block *last, \
			t_block *new)
{
	if (*begin == sort)
	{
		new->next = *begin;
		*begin = new;
		return (new);
	}
	new->next = last->next;
	last->next = new;
	return (new);
}
