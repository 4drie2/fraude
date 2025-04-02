/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 21:25:44 by vboxuser          #+#    #+#             */
/*   Updated: 2024/12/29 21:26:53 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_redir_list(t_redir **redir_list, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (redir_list[i])
		{
			if (redir_list[i]->target)
				free(redir_list[i]->target);
			free(redir_list[i]);
		}
		i++;
	}
	free(redir_list);
}

static void	free_input_redirections(t_command *cmd)
{
	if (cmd->in)
	{
		free_redir_list(cmd->in, cmd->nb_in);
		cmd->in = NULL;
		cmd->nb_in = 0;
	}
}

static void	free_output_redirections(t_command *cmd)
{
	if (cmd->out)
	{
		free_redir_list(cmd->out, cmd->nb_out);
		cmd->out = NULL;
		cmd->nb_out = 0;
	}
}

/*
** free_redirections:
**   Libère toutes les redirections d'entrée et de sortie,
**   qui sont stockées dans cmd->in[i] et cmd->out[i].
*/
void	free_redirections(t_command *cmd)
{
	free_input_redirections(cmd);
	free_output_redirections(cmd);
}
