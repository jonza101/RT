/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 14:28:34 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/15 15:10:12 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*str_free(void)
{
	char	*new;

	if (!(new = (char *)malloc(1)))
		return (NULL);
	new[0] = '\0';
	return (new);
}

char		*ft_strtrim(char const *s)
{
	char	*new;
	size_t	i;
	size_t	j;
	size_t	l;

	if (!s)
		return (NULL);
	l = ft_strlen((char *)s);
	i = 0;
	while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
		i++;
	j = 0;
	while (s[l - 1 - j] == ' ' || s[l - 1 - j] == '\n' || s[l - 1 - j] == '\t')
		j++;
	if (i == l)
	{
		new = str_free();
		return (new);
	}
	if (!(new = (char *)malloc(l - j - i + 1)))
		return (NULL);
	new = ft_strncpy(new, (char *)&s[i], l - (j + i));
	new[l - j - i] = '\0';
	return (new);
}
