/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 17:26:41 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/04 19:47:08 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char c_new;
	unsigned char *b_new;

	c_new = (unsigned char)c;
	b_new = (unsigned char *)b;
	while (len-- > 0)
	{
		*(b_new) = c_new;
		b_new++;
	}
	return (b);
}
