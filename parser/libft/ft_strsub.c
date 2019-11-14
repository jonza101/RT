/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 14:15:05 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/13 14:09:19 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*new;

	i = (size_t)start;
	if (len + 1 == 0)
		return (NULL);
	new = (char *)malloc(len + 1);
	if (!new || !s)
		return (NULL);
	while (s[i] && i < start + len)
	{
		new[i - (size_t)start] = s[i];
		i++;
	}
	new[i - (size_t)start] = '\0';
	return (new);
}
