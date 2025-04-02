/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:51:34 by david             #+#    #+#             */
/*   Updated: 2025/01/22 16:06:59 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_context	*get_signal_context(void)
{
	static t_context	context;

	return (&context);
}

void	child_cleanup_and_exit(int signal)
{
	t_context	*ctx;

	ctx = get_signal_context();
	cleanup_command(ctx->cmd);
	cleanup_heredoc(ctx->state->hd);
	cleanup_state(ctx->state);
	exit(ctx->state->last_exit_status = (127 + signal));
}

static void	child_no_cmd(t_command *cmd, t_state *st)
{
	t_context	*context;

	context = get_signal_context();
	context->cmd = cmd;
	context->state = st;
	if (handle_all_heredocs(cmd, st) < 0)
	{
		free_environment(st->envp);
		free_command_list(cmd);
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, child_cleanup_and_exit);
	if (handle_all_redirections(cmd, st) < 0)
	{
		free_environment(st->envp);
		free_command_list(cmd);
		exit(st->last_exit_status);
	}
	free_environment(st->envp);
	free_command_list(cmd);
	exit(EXIT_SUCCESS);
}

void	handle_no_cmd(t_command *cmd, t_state *state)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		child_no_cmd(cmd, state);
	else
	{
		waitpid(pid, NULL, 0);
		reset_signals_after_execution();
	}
}
