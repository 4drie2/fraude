/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:51:06 by marvin            #+#    #+#             */
/*   Updated: 2025/01/22 15:13:08 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_fork_error(t_state *state, char *path)
{
	char	*exit_str;

	perror("fork error");
	free(path);
	state->last_exit_status = 1;
	exit_str = ft_itoa(state->last_exit_status);
	if (!exit_str)
		return ;
	state->envp = set_env_var(state->envp, "EXIT_STATUS", exit_str);
	free(exit_str);
}

static void	execute_child(t_command *cmd, t_state *st, char *path)
{
	close_child_fds(st);
	init_child_context(cmd, st);
	process_heredocs_or_fail(cmd, st, path);
	signal(SIGINT, child_cleanup_and_exit);
	process_redirections_or_fail(cmd, st);
	setup_signals_for_execution();
	execute_binary_or_fail(cmd, st, path);
}

static void	execute_parent(pid_t pid, t_state *state)
{
	int		status;
	char	*exit_str;

	waitpid(pid, &status, 0);
	reset_signals_after_execution();
	if (WIFEXITED(status))
		state->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		state->last_exit_status = 128 + WTERMSIG(status);
	exit_str = ft_itoa(state->last_exit_status);
	if (!exit_str)
		return ;
	state->envp = set_env_var(state->envp, "EXIT_STATUS", exit_str);
	free(exit_str);
}

static void	execute_fork(t_command *cmd,
	t_state *state)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		handle_fork_error(state, cmd->path);
		return ;
	}
	if (pid == 0)
		execute_child(cmd, state, cmd->path);
	else
		execute_parent(pid, state);
	free(cmd->path);
}

void	handle_external_cmd(t_command *cmd, char **argv, t_state *state)
{
	bool	check;

	check = false;
	if (argv[0] && is_path_absolute_or_relative(argv[0]))
	{
		if (!validate_command_path(argv[0], &cmd->path, state, &check))
		{
			if (check)
				return ;
			handle_command_error(state, argv[0], 127);
			return ;
		}
	}
	else
		cmd->path = get_command_path(argv[0], state);
	if (!cmd->path)
	{
		handle_command_error(state, argv[0], 127);
		return ;
	}
	execute_fork(cmd, state);
}
