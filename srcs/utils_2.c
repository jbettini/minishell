#include "minishell.h"

void	init_cpt(t_cpt *cpt)
{
	cpt->i = 0;
	cpt->j = 0;
	cpt->k = 0;
	cpt->equ = 0;
	cpt->ret = 0;
}

void	exec_in_pipe_child(t_list *cmds, t_var *var, int to_close)
{
	set_sig(SIGINT, SIG_DFL);
	set_sig(SIGQUIT, SIG_DFL);
	if (cmds->next)
		close(to_close);
	dup2(var->in, STDIN_FILENO);
	dup2(var->out, STDOUT_FILENO);
	pipe_routine(cmds->content, var);
}

void	set_next_pipe(t_var *var, int *pipefd)
{
	if (var->in != STDIN_FILENO)
		close(var->in);
	close(pipefd[1]);
	var->in = pipefd[0];
}
