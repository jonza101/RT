/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 14:49:18 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/11 20:38:18 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>
//
static void		free_words(char **array, int n)
{
	int				i;

	i = 0;
	while (i < n)
	{
		free(array[i]);
		i++;
	}
}

unsigned int	k_splits(char const *s, char c)
{
	unsigned int	i;
	unsigned int	k;

	i = 0;
	k = 0;
	if (s[0] != c && s[0] != '\0')
		k++;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			k++;
		i++;
	}
	return (k);
}

static void		split_spaces(const char *str, char **new, char c, int k)
{
	int				i;
	int				y;
	int				x;

	i = 0;
	y = 0;
	x = 0;
	while (x < k)
	{
		if (str[i] != c && str[i] != '\0')
			y++;
		else if (y != 0)
		{
			if (!(new[x] = ft_strsub(str, i - y, y)))
			{
				free_words(new, x);
				free(new);
				return ;
			}
			x++;
			y = 0;
		}
		i++;
	}
}

char			**ft_strsplit(char const *s, char c)
{
	char			**new;
	unsigned int	k;

	if (!s)
		return (NULL);
	k = k_splits(s, c);
	if (!(new = (char **)malloc((k + 1) * sizeof(char *))))
		return (NULL);
	split_spaces(s, new, c, k);
	new[k] = NULL;
	return (new);
}
//
/*
static int		is_separator(char c, char const *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		++i;
	}
	return (0);
}

static int		count_words(char *str, const char *charset)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i] && is_separator(str[i], charset))
		++i;
	if (str[i] && !is_separator(str[i], charset))
		count = 1;
	while (str[i])
	{
		if (!is_separator(str[i], charset) && is_separator(str[i - 1], charset))
			++count;
		++i;
	}
	return (count);
}

static int		word_size(char *str, char const *charset)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (str[i] && !is_separator(str[i], charset))
	{
		++size;
		++i;
	}
	return (size);
}

char			**ft_strsplitchrset(char *str, const char *charset)
{
	char	**tab;
	int		i;
	int		j;
	int		index;

	if (!(tab = (char**)malloc(sizeof(char*) *
							   (count_words(str, charset) + 1))))
		return (NULL);
	i = -1;
	index = -1;
	while (str[++i])
		if (!is_separator(str[i], charset))
		{
			if (index == -1 || is_separator(str[i - 1], charset))
			{
				if (!(tab[++index] = (char*)malloc(sizeof(char) * (word_size(
						str + i, charset) + 1))))
					return (NULL);
				j = 0;
			}
			tab[index][j] = str[i];
			tab[index][++j] = '\0';
		}
	tab[++index] = 0;
	return (tab);
}

char	**ft_strsplit(char const *s, char c)
{
	char	*charset;

	if (!s || !c)
		return (NULL);
	if (!(charset = ft_strnew(2)))
		return (NULL);
	charset[0] = c;
	return (ft_strsplitchrset((char*)s, charset));
}
 */