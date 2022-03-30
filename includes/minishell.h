/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:03:54 by rahmed            #+#    #+#             */
/*   Updated: 2022/03/29 12:59:59 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define	MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include "../libft/libft.h"
# include "cmds.h"
# include <sys/types.h>
# include <sys/wait.h>

# define	PROMPT		"minishell$> "
# define	QUOTE		39
# define	DQUOTE		34
# define	BACKSLASH	92

long long	g_exit_status; // ! A garder ?

enum	e_err
{
	SUCCESS,
	BUILD_ERROR,
	BF_ERROR,
	OP_ERROR,
	DUP_ERROR,
	OUT_ERROR,
	CMD_ERROR = 127
};

enum	e_mod
{
	IN_ENV,
	OUT_OF_ENV,
	IN_PIPE,
	LAST_PIPE_BLOCK,
	IN_MAIN,
	IN_CHILD,
	SET,
	DESTROY_SET
};

enum	e_redir_type
{
	WORD,
	PIPE,
	REDIR_L,
	REDIR_LL,
	REDIR_R,
	REDIR_RR
};
typedef struct s_token
{
	int		type;
	char	*val;
}	t_token;
typedef struct s_env
{
	t_list	*envp;
	char	**nbtfke; // tilteeeeeee
	char	**path;
	char	*cmd_path;
	int		oldstdin;
	int		oldstdout;
	int		out_check;
	int		child;
}		t_env;

/* minishell.c */
t_env	*env_manag(char **env, t_env *to_free, int mod);
int		minishell(t_env *env_set);
int		ft_exit(char **args, int print_exit);
void	ft_export_exit_status(char *arg, t_list **env);
void	wait_this_fk_process(t_env *env);
void	set_path(t_env *env, char **args, int mod);
char	**remake_path(t_env *env, char **args);

/* builtins.c */
int		ft_cmd(char **args, t_env *env, int mod);
int		exec_build_in_env(char **args, t_env *env);
int		ft_pwd(char **args);
void	ft_cd(char **args, t_list **env);
void	ft_write_oldpwd(t_list **env, char *pwd);
char	*ft_my_getenv(t_list **env, char *key);
void	ft_env(char **args, t_env *env, int mod);

/* utils_builtins.c */
int		get_env_name_len(char *str, int getequal);
int		replace_existing_env(char *args, t_list *env);
char	*ft_get_path(char *args);
char	*ft_get_special_path(char *args);
void	my_chdir(char *path, t_list **env);

/* env.c */
void    add_ref(t_list **lst, void *data_ref, int idx);
int		ft_unset(char **arg, t_list **env);
int		ft_export(char **arg, t_list **env);

/* signal.c */
int		handle_eof(char *str);
void	set_sig(int signum, void (*handler)(int));
void	sigint_handler(int signum);
void	sigquit_handler(int signum);

/* connect.c */
int		redir_manag(t_redir *to_redir, t_env *env);
int		redir_lst(t_list *redir_lst, t_env *env);
// int		redir_all(t_list *in, t_list *out, t_env *env);
void	error_manag(int ret);
int		launch_exec(t_env *env, t_cmd *cmd, int mod);
int		exec_block(t_cmd *to_exec, t_env *env, int mod);
int		connecting_fct(t_list *line, t_env *env);

/* exec.c */
int		ft_isbuild(char *args);
int		exec_in_main(t_cmd *cmd, t_env *env, int mod);
int		execute_cmd(char **args, t_env *env, int mod);
int		exec_in_child(char **args, t_env *env, int mod);
char	*parse_cmd(char **path, char **cmd);

/* redir.c */
int		redir_heredoc(char *stop);
char	**here_doc(char *stop);
int		redir_to_stdout(void *file, int mod);
int		redir_to_stdin(void *file);

/* echo.c */
void	ft_echo(char **arg);

/* pipe.c */
void	ft_pipex(t_cmd *cmd, t_env *env);

/* miniparser.c */
int		ft_strequ_h(char const *s1, char const *s2);
int		is_notredir(char *tab);
int		tmp_utils(t_cmd **cmd, char **str, int mod);
t_list	*redir(char *tmp, char *file);
void	free_redir(void *ptr);
void	destroy_biglst_content(t_list **lst);
int		cmd_parser(t_cmd **cmd, char **tab);
t_list	*cmd_maker(char *str);
t_list	*miniparser(char *str);
void	ft_print_dpt(char **split);
void	print_redir_lst(t_list *lst);
void	ft_putbiglst(t_list *lst);

/* dev.c */
void	print_tokens(t_list *tokens);
void	print_redir(void *ptr);
void	print_cmd(void *ptr);

/* error.c */
void	print_error(char *msg);
void	exit_error(char *msg, int code);
int		error(char *msg, int code);
void	*error_null(char *msg);

/* expand_ev.c */
// static char	*get_ev_name(char *ev);
// static char	*get_ev_value(char **local_env, char *ev_name);
// static void	rearrange_line(char **line, int *i, char **local_env);
int	expand_ev(t_list *cmds, t_env *env_set);

/* free.c */
void	free_token(void *ptr);
// static void	free_redir(void *ptr);
void	free_cmd(void *ptr);

/* get_cmds.c */
// static void	init_redir(t_redir **redir, int type);
// static int	add_redir(t_cmd *cmd, t_list **tokens);
// static void	init_cmd(t_cmd **cmd);
// static t_cmd	*get_next_cmd(t_list **tokens);
t_list	*get_cmds(t_list *tokens);

/* get_tokens.c */
// static int	get_len_word(char *line);
// static int	extract_word(char **line, t_token *tok);
// static int	extract_symbol(char **line, t_token *tok);
// static t_token	*get_next_token(char **line);
t_list	*get_tokens(char *line);

/* parse.c */
t_list	*parse(char *line);

/* strs.c */
void	print_strs(char **strs);
void	free_strs(char **strs);
char	**copy_strs(char **strs);
int		strs_len(char **strs);
char	**strs_append(char **strs, const char *str);
char	**strs_join(char **strs1, char **strs2);

/* utils.c */
int		is_symbol(char c);
int		is_whitespace(char c);
void	skip_whitespace(char **line);
void	my_strncpy(char *dst, const char *src, int n);
int		my_strcmp(char *s1, char *s2);
char	*str_insert(char *dst, const char *src, size_t n);
char	*get_str_truncated(const char *str, int start, int len);
char	*trunc_str(char *str, int start, int len);
int		get_token_type(t_token *tok);
char	*get_token_value(t_token *tok);

/* ft_strtok.c */
char	**ft_strtok(char *str, char *delim);

#endif
