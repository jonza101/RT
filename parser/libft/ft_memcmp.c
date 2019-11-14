/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 23:16:46 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/09 13:29:49 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char		*s1_new;
	const unsigned char		*s2_new;

	s1_new = (unsigned char *)s1;
	s2_new = (unsigned char *)s2;
	while (n-- > 0)
	{
		if (*s1_new != *s2_new)
			return (*s1_new - *s2_new);
		s1_new++;
		s2_new++;
	}
	return (0);
}
