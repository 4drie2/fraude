/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:11:02 by david             #+#    #+#             */
/*   Updated: 2025/01/22 16:08:31 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_child_fds(t_state *st)
{
	close(st->o_stdin);
	close(st->o_stdout);
}

void	init_child_context(t_command *cmd, t_state *st)
{
	t_context	*ctx;

	ctx = get_signal_context();
	ctx->cmd = cmd;
	ctx->state = st;
}

void	process_heredocs_or_fail(t_command *cmd, t_state *st, char *path)
{
	if (handle_all_heredocs(cmd, st) < 0)
	{
		free(path);
		free_environment(st->envp);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		exit(EXIT_FAILURE);
	}
}

void	process_redirections_or_fail(t_command *cmd, t_state *st)
{
	if (handle_all_redirections(cmd, st) < 0)
	{
		free(cmd->path);
		free_environment(st->envp);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		exit(st->last_exit_status);
	}
}

void	execute_binary_or_fail(t_command *cmd, t_state *st, char *path)
{
	if (execve(path, cmd->args, st->envp) == -1)
	{
		free(path);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_environment(st->envp);
		exit(EXIT_FAILURE);
	}
}
