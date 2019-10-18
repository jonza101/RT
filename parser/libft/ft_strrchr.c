/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 00:27:30 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/09 18:46:56 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	size_t	lastpos;

	i = 0;
	lastpos = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			lastpos = i;
		i++;
	}
	if (c == '\0' && s[i] == '\0')
		return ((char *)(s + i));
	if (lastpos != 0 || (lastpos == 0 && s[0] == (char)c))
		return ((char *)(s + lastpos));
	else
		return (NULL);
}
