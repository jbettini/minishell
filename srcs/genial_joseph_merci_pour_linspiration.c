/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genial_joseph_merci_pour_linspiration.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 17:28:37 by ydanset           #+#    #+#             */
/*   Updated: 2022/05/05 17:28:55 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	genial_joseph_merci_pour_linspiration(t_cpt *t)
{
	print_error(ft_strdup("export"), "not a valid identifier");
	t->ret = BUILD_ERROR;
}
