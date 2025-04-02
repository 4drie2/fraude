/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 08:14:07 by marvin            #+#    #+#             */
/*   Updated: 2025/01/15 08:14:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_builtins(t_command *cmd, t_state *state)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (handle_exit(cmd, state), 2);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (handle_cd_command(cmd->args, state), 1);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (handle_pwd_command(state), 1);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (handle_env_command(state->envp, state), 1);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (handle_echo_command(cmd, state), 1);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (handle_unset_command(cmd->args, state->envp), 1);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (handle_export_command(cmd->args, state), 1);
	else if (ft_strcmp(cmd->args[0], "neosnitch") == 0)
		return (handle_neosnitch_command(cmd->args, state), 1);
	else if (ft_strcmp(cmd->args[0], ".") == 0)
		return (handle_source_command(cmd->args, state), 1);
	return (0);
}

void	handle_source_command(char **args, t_state *state)
{
	if (!args[1])
	{
		ft_putstr_fd(".: command not found\n", 2);
		state->last_exit_status = 127;
		return ;
	}
}

void	reset_state(t_state *state)
{
	state->num_pipes = 0;
	state->index = 0;
	state->o_stdout = -1;
	state->o_stdin = -1;
	state->pids = NULL;
	state->pipes = NULL;
}

// Vérifie la syntaxe de la commande initiale
int	check_syntax(t_command *start_cmd, t_state *state)
{
	if (!start_cmd->args[0] && start_cmd->is_pipe)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		state->last_exit_status = 2;
		return (0);
	}
	return (1);
}
