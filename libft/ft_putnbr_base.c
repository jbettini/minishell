/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 17:56:13 by jbettini          #+#    #+#             */
/*   Updated: 2021/06/22 18:04:00 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include<unistd.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putbase(long nb, char *base)
{
	write(1, &base[nb % ft_strlen(base)], 1);
}

int	check_base(char *str)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-' || str[i] == '+')
			return (0);
		i++;
	}
	i = 0;
	while (str[i])
	{
		tmp = str[i];
		j = i + 1;
		while (str[j])
		{
			if (tmp == str[j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	ft_putnbr_base(int nbr, char *base)
{
	int		i;
	long	nb;

	i = 0;
	nb = nbr;
	if (ft_strlen(base) >= 2)
	{
		i = check_base(base);
		if (i == 1)
		{
			if (nb < 0)
			{
				write(1, "-", 1);
				nb *= -1;
			}
			if (nb > ft_strlen(base))
				ft_putnbr_base(nb / ft_strlen(base), base);
			ft_putbase(nb, base);
		}
	}
}
