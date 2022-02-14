/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 20:16:21 by jbettini          #+#    #+#             */
/*   Updated: 2021/06/15 12:30:12 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_sqrt(int nb)
{
	long	i;
	long	res;

	i = 1;
	res = 0;
	if (nb <= 0)
		return (0);
	while (res < nb)
	{
		res = i * i;
		if (res == nb)
			return (i);
		i++;
	}
	return (0);
}

/*int main ()
{


	int i = 2147483647 ;
	int r = 0;
	r = ft_sqrt(i);
	printf("%d", r);
}*/
