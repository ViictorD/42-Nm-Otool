/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 20:12:15 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/02 00:07:43 by vdarmaya         ###   ########.fr       */
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

t_block		*new_block(int i, char *name)
{
	t_block	*new;

	if (!(new = (t_block*)malloc(sizeof(t_block))))
		ft_exiterror("Malloc failed\n", 1);
	new->index = i;
	new->name = ft_strdup(name);
	new->next = NULL;
	return (new);
}

void		include_before(t_block **begin, t_block *sort, t_block *last, \
			t_block *new)
{
	if (*begin == sort)
	{
		new->next = *begin;
		*begin = new;
		return ;
	}
	new->next = last->next;
	last->next = new;
}

t_sorted	*get_index_block(t_block *begin)
{
	int			i;
	t_block		*b;
	t_sorted	*sorted;

	i = -1;
	b = begin;
	while (b)
	{
		++i;
		b = b->next;
	}
	if (!(sorted = (t_sorted*)malloc(sizeof(t_sorted))))
		ft_exiterror("Malloc failed\n", 1);
	if (!(sorted->count = (int*)malloc(sizeof(int) * i)))
		ft_exiterror("Malloc failed\n", 1);
	i = -1;
	b = begin;
	while (b)
	{
		sorted->count[++i] = b->index;
		b = b->next;
	}
	sorted->tab_size = i;
	return (sorted);
}
