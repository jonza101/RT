/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 22:10:57 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/09 18:50:18 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*dst_new;
	const unsigned char	*src_new;
	size_t				i;

	if (!src && !dst)
		return (NULL);
	i = 0;
	dst_new = (unsigned char *)dst;
	src_new = (unsigned char *)src;
	if (src_new > dst_new)
	{
		while (i < len)
		{
			dst_new[i] = src_new[i];
			i++;
		}
	}
	else
	{
		while (len-- > 0)
			dst_new[len] = src_new[len];
	}
	return (dst);
}
