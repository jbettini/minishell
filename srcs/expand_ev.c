/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:15 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/13 19:16:23 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_ev_name(char *ev)
{
	int		i;
	char	*ev_name;

	if (ev[0] == '?')
	{
		ev_name = ft_strdup("?");
		if (!ev_name)
			exit_error("cannot allocate memory", EXIT_FAILURE);
		return (ev_name);
	}
	if (ft_isdigit(ev[0]))
	{
		ev_name = ft_calloc(2, 1);
		if (!ev_name)
			exit_error("cannot allocate memory", EXIT_FAILURE);
		ev_name[0] = ev[0];
		return (ev_name);
	}
	i = 0;
	while (ev[i] && (ft_isalnum(ev[i]) || ev[i] == '_'))
		i++;
	ev_name = malloc(i + 1);
	if (!ev_name)
		exit_error("cannot allocate memory", EXIT_FAILURE);
	my_strncpy(ev_name, ev, i);
	return (ev_name);
}

static char	*get_ev_value(char **local_env, char *ev_name)
{
	char	*ev_value;
	char	*tmp;
	int		i;

	i = 0;
	while (local_env[i])
	{
		tmp = get_ev_name(local_env[i]);
		if (!my_strcmp(tmp, ev_name))
		{
			ev_value = get_str_truncated(local_env[i], 0, ft_strlen(ev_name) + 1);
			if (!ev_value)
				exit_error("cannot allocate memory", EXIT_FAILURE);
			free(tmp);
			return (ev_value);
		}
		free(tmp);
		i++;
	}
	ev_value = ft_strdup("");
	if (!ev_value)
		exit_error("cannot allocate memory", EXIT_FAILURE);
	return (ev_value);
}

static void	rearrange_line(char **line, int *i, char **local_env)
{
	char	*ev_name;
	char	*ev_value;

	ev_name = get_ev_name(&(*line)[*i + 1]);
	*line = trunc_str(*line, *i + 1, ft_strlen(ev_name));
	if (!*line)
		exit_error("cannot allocate memory", EXIT_FAILURE);
	if (!my_strcmp(ev_name, "?"))
	{
		ev_value = ft_itoa(g_exit_status);
		if (!ev_value)
			exit_error("cannot allocate memory", EXIT_FAILURE);
	}
	else
		ev_value = get_ev_value(local_env, ev_name);
	*line = str_insert(*line, ev_value, *i);
	if (!line)
		exit_error("cannot allocate memory", EXIT_FAILURE);
	*i += ft_strlen(ev_value);
	free(ev_value);
	free(ev_name);
}

void	expand_ev(char **line, char **local_env)
{
	int		i;

	// char	*ev_name; // ! unused
	// char	*ev_value; // ! unused
	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '$')
		{
			if ((*line)[i + 1] == '?' || ft_isalnum((*line)[i + 1]) || (*line)[i + 1] == '_')
				rearrange_line(line, &i, local_env);
			else
				i++;
		}
		else if ((*line)[i++] == '\'')
			while ((*line)[i] && (*line)[i] != '\'')
				i++;
	}
}
