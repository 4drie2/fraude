/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:58:50 by david             #+#    #+#             */
/*   Updated: 2025/04/02 10:57:09 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_single_command(t_command **cmd,
	t_state *st, t_fork_info *fi, int i)
{
	if (handle_all_heredocs(*cmd, st) < 0)
		return (-1);
	signal(SIGINT, child_cleanup_and_exit);
	fi->pids[i] = fork();
	if (fi->pids[i] == -1)
	{
		perror("fork");
		return (-1);
	}
	if (fi->pids[i] == 0)
		child_process(*cmd, st, fi, i);
	reset_signals_after_execution();
	*cmd = (*cmd)->next;
	return (0);
}

void	child_process(t_command *cmd, t_state *st,
	t_fork_info *fi, int i)
{
	if (i > 0)
		dup2(fi->pipes[i - 1][0], STDIN_FILENO);
	if (i < fi->num_cmds - 1)
		dup2(fi->pipes[i][1], STDOUT_FILENO);
	close_all_pipes(fi->num_cmds - 1, fi->pipes);
	free(fi->pipes);
	free(fi->pids);
	if (cmd->args[0])
	{
		execute_command(cmd, st);
		exit(st->last_exit_status);
	}
	else if (!cmd->args[0] || !cmd->args[0][0])
		handle_empty_cmd(cmd, st);
	exit(127);
}

void	handle_empty_cmd(t_command *cmd, t_state *st)
{
	if (handle_all_redirections(cmd, st) < 0)
	{
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_environment(st->envp);
		exit(st->last_exit_status = 1);
	}
	while (cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	free_environment(st->envp);
	exit(st->last_exit_status);
}
