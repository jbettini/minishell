/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 21:15:24 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/05 17:13:22 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_ev_name(char *str)
{
	int		i;
	char	*ev_name;

	if (str[0] == '?')
		return (ft_strdup("?"));
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	ev_name = malloc(i + 1);
	my_strncpy(ev_name, str, i);
	return (ev_name);
}

char	*get_ev_value(char *ev_name, char **env)
{
	char	*ev_value;
	char	*tmp;
	int		i;

	i = 0;
	while (env[i])
	{
		tmp = get_ev_name(env[i]);
		if (!my_strcmp(tmp, ev_name))
		{
			ev_value = get_str_truncated(env[i], 0, ft_strlen(ev_name) + 1);
			free(tmp);
			return (ev_value);
		}
		free(tmp);
		i++;
	}
	ev_value = ft_strdup("");
	return (ev_value);
}

void	rearrange_word(char **word, int *i, t_var *var)
{
	char	*ev_name;
	char	*ev_value;
	char	**local_env;

	ev_name = get_ev_name(&(*word)[*i + 1]);
	*word = trunc_str(*word, *i + 1, ft_strlen(ev_name));
	if (!my_strcmp(ev_name, "?"))
		ev_value = ft_itoa(g_glb.exit_status);
	else
	{
		local_env = ft_lst_to_dpt(var->local_env);
		ev_value = get_ev_value(ev_name, local_env);
		free_strs(local_env);
	}
	*word = str_insert(*word, ev_value, *i);
	*i += ft_strlen(ev_value);
	free(ev_value);
	free(ev_name);
}

void	delete_quotes(char **word)
{
	int		i;
	char	quote;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'' || (*word)[i] == '"')
		{
			quote = (*word)[i];
			*word = trunc_str(*word, i, 1);
			while ((*word)[i] && (*word)[i] != quote)
				i++;
			if ((*word)[i])
				*word = trunc_str(*word, i, 1);
		}
		else
			i++;
	}
}
