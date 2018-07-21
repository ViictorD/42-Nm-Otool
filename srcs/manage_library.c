/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_library.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 13:12:20 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 15:52:03 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned long	parse_nb(void *ptr)
{
	char			buff[9];
	unsigned int	i;

	ft_bzero(buff, 9);
	i = 0;
	while (*(char*)(ptr + i) != 32)
	{
		buff[i] = *(char*)(ptr + i);
		++i;
	}
	return (ft_atoi(buff));
}

static unsigned int		get_name_space(unsigned int len)
{
	unsigned int	count;

	count = 12;
	while (len + 4 > count)
		count += 8;
	return (count);
}

void					manage_library(void *ptr, unsigned int filesize, \
							char *name)
{
	unsigned long	size;
	void			*tmp;
	unsigned int	name_len;
	unsigned int	obj_size;

	size = parse_nb(ptr + 0x38);
	tmp = ptr + 0x44 + size;
	while (tmp - ptr < filesize)
	{
		obj_size = parse_nb(tmp + HEAD_OBJ_SIZE);
		ft_putchar('\n');
		ft_putstr(name);
		ft_putchar('(');
		ft_putstr((char*)(tmp + HEAD_OBJ_NAME));
		ft_putstr("):\n");
		name_len = ft_strlen((char*)(tmp + HEAD_OBJ_NAME));
		name_len = get_name_space(name_len);
		manage_nm(tmp + HEAD_OBJ_END + name_len, obj_size - name_len, name);
		tmp += HEAD_OBJ_END + obj_size;
	}
}
