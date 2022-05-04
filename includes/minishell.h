/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 19:51:20 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 19:02:00 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <limits.h>
# include <sys/errno.h>

# define PROMPT		"\033[0;32mminishell$> \033[0m"
# define QUOTE		39
# define DQUOTE		34
# define BACKSLASH	92

typedef struct s_global
{
	long long	exit_status;
	int			sigint_in_hd;
	int			in_hd;
}	t_global;

t_global	g;

typedef struct s_cpt
{
	int	i;
	int	j;
	int	k;
	int	ret;
	int	equ;
}	t_cpt;

enum	e_err_cmd
{
	CTRL_C = -2,
	CTRL_D,
	SUCCESS,
	BUILD_ERROR,
	BF_ERROR,
	OP_ERROR,
	DUP_ERROR,
	OUT_ERROR,
	EXPAND_ERROR,
	PERM_ERROR,
	CMD_ERROR = 127
};

enum	e_mode
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

typedef struct s_var
{
	t_list	*local_env;
	t_list	*local_export;
	t_list	*hd_to_unlink;
	char	**envp;
	char	**path;
	char	*cmd_path;
	int		oldstdin;
	int		oldstdout;
	int		child;
	int		last_pid;
	int		in;
	int		out;
	int		pid;
	struct termios	usr_tty_config;
}		t_var;

int		exec_pipe(t_list *cmds, t_var *var);
int		redir_all(t_cmd *cmd, t_var *var);
int		all_error(int ret, char *error);
int		exec_build(char **args, t_var *var);
int		exec_multiple_cmds(t_list *cmds, t_var *var);
void	exec_in_pipe_child(t_list *cmds, t_var *var, int to_close);
void	set_next_pipe(t_var *var, int *pipefd);
void	pipe_routine(t_cmd *cmd, t_var *var);
void	cd_to_envvar(t_var *var, char *ev_name);
void	my_chdir(char *path, t_var *var);
void	ft_cd(char **args, t_var *var);
int		exec_build_in_env(char **args, t_var *var);
void	ft_echo(char **arg);
int		ft_exit(char **args, int print_exit, t_var *var);
int		check_the_build_for_env(char *args);
void	check_unset_path(char **path, t_var *var);
int		ft_isbuild(char *args);     
void	set_path(t_var *var, char **args, int mode);
void	reset_routine(t_var *var, int ret);
int		redir_manager(t_redir *to_redir);
int		redir_lst(t_list *redir_lst, t_var *var);
int		launch_exec(t_var *var, t_cmd *cmd, int mode);
void	reset_routine_mc(t_var *var, int mode);
int		exec_simple_cmd(t_cmd *cmd, t_var *var);
int		exec_cmds(t_list *cmds, t_var *var);
int		is_valide_var(char *str, int mode);
int		ft_strc_index(char *str, int c);
void	delref(t_list **lst, void *data_ref);
void	add_ref(t_list **lst, void *data_ref, int idx, int mode);
void	ft_putexport(t_list *lst);
void	ft_env(char **args, t_var *var);
int		ft_pwd(char **args);
int		ft_unset(char **arg, t_var *var);
int		ft_export(char **arg, t_var *var);
void	print_error(char *cmd, const char *msg);
void	exit_error(char *cmd, const char *msg, int code);
int		error(char *cmd, const char *msg, int code);
void	*error_null(char *cmd, const char *msg);
void	expand_word(char **word, t_var *var);
char	**expand_args(char **args, t_var *var);
int		redir_expanded_is_valid(char *word_expanded);
int		expand_redir(t_redir *redir, t_var *var);
char	*get_ev_name(char *str);
char	*get_ev_value(char *ev_name, char **env);
void	rearrange_word(char **word, int *i, t_var *var);
void	delete_quotes(char **word);
void	free_token(void *ptr);
void	free_redir(void *ptr);
void	free_cmd(void *ptr);
char	**ft_strtok(char *str, char *delim);
char	*get_next_line_hd(int fd);
t_list	*get_cmds(t_list *tokens);
t_list	*get_tokens(char *line);
int		get_len_word(char *line);
t_var	*var_manager(char **env, t_var *to_free, int mode);
void	wait_this_fk_process(t_var *var);
int		minishell(t_var *var);
int		main(int ac, char **av, char **env);
t_list	*parse(char *line);
char	*parse_cmd(char **path, char **cmd);
void	init_cpt(t_cpt *cpt);
int		redir_to_stdout(void *file, int mode);
int		redir_to_stdin(void *file);
void	sigint_handler(int signum);
void	set_sig(int signum, void (*handler)(int));
void	free_strs(char **strs);
char	**copy_strs(char **strs);
int		strs_len(char **strs);
char	**strs_append(char **strs, const char *str);
char	**strs_join(char **strs1, char **strs2);
void	save_usr_tty_config(t_var *var);
void	reset_usr_tty_config(t_var *var);
void	tty_hide_ctrl(void);
void	tty_show_ctrl(void);
char	*str_insert(char *dst, const char *src, size_t n);
char	*get_str_truncated(const char *str, int start, int len);
char	*trunc_str(char *str, int start, int len);
int		get_token_type(t_token *tok);
char	*get_token_value(t_token *tok);
int		is_symbol(char c);
int		is_whitespace(char c);
void	skip_whitespace(char **line);
void	my_strncpy(char *dst, const char *src, int n);
int		my_strcmp(char *s1, char *s2);
void	unlink_all(t_var *var);
int		convert_a_hd(t_redir *redir);
int		convert_all_hd(t_list *r_in, int i, t_var *var);
int		hd_to_infile(t_list *cmds, t_var *var);

#endif
