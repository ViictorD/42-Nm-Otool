/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 15:11:10 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/07/21 15:12:24 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	is_ppc_arch(char set, char value)
{
	static	char	ppc = 0;

	if (set)
		ppc = value;
	return (ppc);
}

char	get_rev(char set, char value)
{
	static char rev = 0;

	if (set)
		rev = value;
	return (rev);
}
