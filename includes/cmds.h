/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 23:30:50 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 08:23:24 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

typedef struct s_redir
{
	int		type;
	char	*keyword;
	char	*filename;
}	t_redir;

typedef struct s_cmd
{
	char	**args;
	t_list	*redirs;
}	t_cmd;

#endif