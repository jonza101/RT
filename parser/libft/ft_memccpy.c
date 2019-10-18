/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 21:33:59 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/09 12:35:54 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memccpy(void *restrict_dst,
		const void *restrict_src, int c, size_t n)
{
	unsigned char		*dest;
	const unsigned char	*src;
	size_t				i;
	unsigned char		char_c;

	char_c = (unsigned char)c;
	dest = (unsigned char *)restrict_dst;
	src = (unsigned char *)restrict_src;
	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		if (src[i] == char_c)
			return (restrict_dst + i + 1);
		i++;
	}
	return (NULL);
}
