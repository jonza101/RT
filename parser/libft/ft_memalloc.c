/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 13:00:07 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/07 17:03:26 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_memalloc(size_t size)
{
	void *new;

	new = malloc(size);
	if (new == 0)
		return (0);
	ft_bzero(new, size);
	return (new);
}
