/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 23:30:50 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/01 18:44:52 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

typedef struct s_redir
{
	int		type;
	char	*word;
}	t_redir;

typedef struct s_cmd
{
	char	**args;
	t_list	*redir_in;
	t_list	*redir_out;
}	t_cmd;

#endif