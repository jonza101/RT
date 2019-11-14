/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 23:00:30 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/07 16:13:40 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char		*s_new;
	size_t				count;
	unsigned char		c_new;

	s_new = (unsigned char *)s;
	c_new = (unsigned char)c;
	count = 0;
	while (n-- > 0)
	{
		if (*s_new == c_new)
			return (s_new);
		count++;
		s_new++;
	}
	return (NULL);
}
