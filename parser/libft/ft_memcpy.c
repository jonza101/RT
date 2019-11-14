/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 21:03:08 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/07 18:00:09 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *destptr, const void *srcptr, size_t num)
{
	unsigned char		*dest;
	const unsigned char	*src;

	if (!destptr && !srcptr)
		return (NULL);
	dest = (unsigned char *)destptr;
	src = (unsigned char *)srcptr;
	while (num-- > 0)
	{
		*dest = *src;
		dest++;
		src++;
	}
	return (destptr);
}
