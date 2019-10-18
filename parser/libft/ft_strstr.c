/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 00:13:31 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/14 00:02:45 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *str, const char *to_find)
{
	int flag;
	int i;
	int j;

	i = 0;
	if (ft_strlen(to_find) == 0)
		return ((char *)str);
	while (str[i] != '\0')
	{
		if (str[i] == to_find[0])
		{
			j = 0;
			flag = 1;
			while (to_find[j] != '\0')
			{
				if (to_find[j] != str[i + j])
					flag = 0;
				j++;
			}
			if (flag == 1)
				return ((char *)&str[i]);
		}
		i++;
	}
	return (0);
}
