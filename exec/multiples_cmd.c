/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:29:25 by vboxuser          #+#    #+#             */
/*   Updated: 2025/01/22 15:55:40 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_builtin_part1(t_command *cmd, t_state *temp_state)
{
	if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		handle_env_command(temp_state->envp, temp_state);
		if (temp_state->last_exit_status)
			temp_state->last_exit_status = 1;
		else
			temp_state->last_exit_status = 0;
	}
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		handle_export_command(cmd->args, temp_state);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		if (cmd->args[1] && cmd->args[2])
		{
			free_environment(temp_state->envp);
			while (cmd->prev)
				cmd = cmd->prev;
			free_command_list(cmd);
			print_error_and_exit("exit: ", "", "too many arguments\n", 1);
		}
		handle_exit(cmd, temp_state);
	}
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		handle_cd_command(cmd->args, temp_state);
}

static void	execute_builtin_part2(t_command *cmd, t_state *temp_state)
{
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		handle_pwd_command(temp_state);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		handle_echo_command(cmd, temp_state);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		handle_unset_command(cmd->args, temp_state->envp);
	else if (ft_strcmp(cmd->args[0], "neosnitch") == 0)
		handle_neosnitch_command(cmd->args, temp_state);
}

void	execute_builtin(t_command *cmd, t_state *temp_state)
{
	if (ft_strcmp(cmd->args[0], "env") == 0
		|| ft_strcmp(cmd->args[0], "export") == 0
		|| ft_strcmp(cmd->args[0], "exit") == 0
		|| ft_strcmp(cmd->args[0], "cd") == 0)
		execute_builtin_part1(cmd, temp_state);
	else
		execute_builtin_part2(cmd, temp_state);
	return ;
}

void	execute_command(t_command *cmd, t_state *state)
{
	t_state	temp_state;
	char	**temp_env;

	handle_redirections_or_exit(cmd, state);
	copy_environment_or_exit(state, &temp_state, &temp_env);
	handle_no_args_or_exit(cmd, state, temp_env);
	if (is_builtin(cmd->args[0]))
	{
		free_environment(state->envp);
		execute_builtin(cmd, &temp_state);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_environment(temp_env);
		exit(state->last_exit_status);
	}
	free_environment(state->envp);
	execute_external(cmd, &temp_state);
	free_environment(temp_env);
}

void	handle_multiples_pipes(t_command *start_cmd, t_state *state)
{
	state->num_pipes = allocate_and_create_pipes(start_cmd,
			&state->pipes, &state->pids, state);
	if (state->num_pipes == -1)
		return ;
	execute_and_cleanup(start_cmd, state,
		(int (*)[2])state->pipes, state->pids);
}
