/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 23:30:50 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/13 23:30:57 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

// # include "libft.h"

typedef struct	s_redir
{
	int		type; // WORD | PIPE | REDIR_L | REDIR_LL | REDIR_R | REDIR_RR
	char	*word;
}	t_redir;

/* OLD*/
// typedef struct	s_redir
// {
// 	char	*type; // "<" or "<<" or ">" or ">>"
// 	char	*filename;
// }	t_redir;

typedef struct	s_cmd
{
	char	**args; // args[0] = command
	t_list	*redir_in; // '<' and '<<' ; ordered from first seen to last seen
	t_list	*redir_out; // '>' and '>>' ; ordered from first seen to last seen
	// 'redir_in/out->content' will be a pointer on a 't_redir' structure
}	t_cmd;

/* OLD*/
// typedef struct	s_cmd
// {
// 	char	*name;
// 	char	**args; // args[0] = name
// 	t_list	*redir_in; // '<' and '<<' ; ordered from first seen to last seen
// 	t_list	*redir_out; // '>' and '>>' ; ordered from first seen to last seen
// }	t_cmd;

/*
	. your execution fct will receive a 't_list *cmds' variable

	. 'cmds->content' will be a pointer on a 't_cmd' structure

	. between each 'cmds' elements we assume there is a pipe
*/

#endif
