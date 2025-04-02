/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 05:40:35 by vboxuser          #+#    #+#             */
/*   Updated: 2024/12/19 05:40:35 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unlink_all_heredocs(t_command *cmd)
{
	int	i;

	while (cmd)
	{
		i = 0;
		while (i < cmd->nb_in)
		{
			if (cmd->in[i]->type == R_HEREDOC && cmd->in[i]->target)
			{
				unlink(cmd->in[i]->target);
				free(cmd->in[i]->target);
				cmd->in[i]->target = NULL;
			}
			i++;
		}
		cmd = cmd->next;
	}
}

static int	save_original_fds(t_state *state)
{
	state->o_stdin = dup(STDIN_FILENO);
	if (state->o_stdin == -1)
	{
		perror("dup");
		return (-1);
	}
	state->o_stdout = dup(STDOUT_FILENO);
	if (state->o_stdout == -1)
	{
		perror("dup");
		close(state->o_stdin);
		return (-1);
	}
	return (0);
}

/*
** Restaure les descripteurs de fichiers originaux et les ferme.
*/
void	restore_and_close_fds(t_state *state)
{
	if (state->o_stdin && dup2(state->o_stdin, STDIN_FILENO) == -1)
		perror("dup2");
	if (state->o_stdout && dup2(state->o_stdout, STDOUT_FILENO) == -1)
		perror("dup2");
	if (state->o_stdin)
		close(state->o_stdin);
	if (state->o_stdout)
		close(state->o_stdout);
}

/*
Exécute une commande sans pipeline, avec gestion des redirections et des built.
*/
static void	exec_single_command(t_command *cmd, t_state *st)
{
	if (save_original_fds(st) == -1)
		return ;
	if (exec_if_builtin(cmd, st))
		return ;
	if (check_if_empty_command(cmd, st))
		return ;
	handle_external_cmd(cmd, cmd->args, st);
	restore_and_close_fds(st);
}

/*
** Fonction principale pour l'exécution des commandes.
*/
void	exec_shell(t_command *cmd, t_state *state)
{
	if ((!cmd->args || !cmd->args[0]) && !cmd->is_pipe)
	{
		handle_no_cmd(cmd, state);
	}
	else if (!cmd->is_pipe)
		exec_single_command(cmd, state);
	else
		handle_multiples_pipes(cmd, state);
}
