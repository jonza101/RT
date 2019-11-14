/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 02:55:32 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/09 19:37:24 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	int				x;
	unsigned int	i;

	i = 0;
	x = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && (i < n))
	{
		if (s1[i] != s2[i])
		{
			x = (unsigned char)s1[i] - (unsigned char)s2[i];
			return (x);
		}
		i++;
	}
	return (0);
}
