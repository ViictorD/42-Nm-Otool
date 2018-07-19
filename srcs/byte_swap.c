/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   byte_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 22:01:34 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/19 18:40:13 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static unsigned short	uswap_16(unsigned short x)
{
	return (((((x) >> 8) & 0xffu) | (((x) & 0xffu) << 8)));
}

int						swap_32(int x)
{
	return (((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >>  8) | \
			(((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24)));
}

unsigned int			uswap_32(unsigned int x)
{
	return (((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >>  8) | \
			(((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24)));
}

unsigned long			uswap_64(unsigned long x)
{
	return ((((x) & 0xff00000000000000ull) >> 56) \
		| (((x) & 0x00ff000000000000ull) >> 40) \
		| (((x) & 0x0000ff0000000000ull) >> 24) \
		| (((x) & 0x000000ff00000000ull) >> 8) \
		| (((x) & 0x00000000ff000000ull) << 8) \
		| (((x) & 0x0000000000ff0000ull) << 24) \
		| (((x) & 0x000000000000ff00ull) << 40) \
		| (((x) & 0x00000000000000ffull) << 56));
}

char					get_rev(char set, char value)
{
	static char rev = 0;

	if (set)
		rev = value;
	return (rev);
}

unsigned long			swap_bits(unsigned long value, int base)
{
	if (get_rev(0, 0))
	{
		if (base == 16)
			return (uswap_16(value));
		else if (base == 32)
			return (uswap_32(value));
		else if (base == 64)
			return (uswap_64(value));
	}
	return (value);
}
