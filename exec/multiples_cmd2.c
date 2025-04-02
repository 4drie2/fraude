/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples_cmd2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:45:10 by david             #+#    #+#             */
/*   Updated: 2025/01/22 15:56:29 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirections_or_exit(t_command *cmd, t_state *st)
{
	if (handle_all_redirections(cmd, st) < 0)
	{
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_environment(st->envp);
		exit(st->last_exit_status = 1);
	}
}

void	copy_environment_or_exit(t_state *st,
	t_state *temp_st, char ***temp_env)
{
	*temp_env = copy_environment(st->envp);
	if (!(*temp_env))
		exit(1);
	*temp_st = *st;
	temp_st->envp = *temp_env;
}

void	handle_no_args_or_exit(t_command *cmd,
	t_state *st, char **temp_env)
{
	if (!cmd->args || !cmd->args[0])
	{
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_environment(st->envp);
		free_environment(temp_env);
		exit(EXIT_SUCCESS);
	}
}
