/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 22:25:05 by marvin            #+#    #+#             */
/*   Updated: 2024/12/14 22:25:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

/*
** Libère les redirections d'entrée (in) d'une commande.
*/
static void	free_input_redirections(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_in)
	{
		if (cmd->in[i])
		{
			free(cmd->in[i]->target);
			free(cmd->in[i]);
			cmd->in[i] = NULL;
		}
		i++;
	}
	free(cmd->in);
	cmd->in = NULL;
}

/*
** Libère les redirections de sortie (out) d'une commande.
*/
static void	free_output_redirections(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_out)
	{
		if (cmd->out[i])
		{
			free(cmd->out[i]->target);
			free(cmd->out[i]);
			cmd->out[i] = NULL;
		}
		i++;
	}
	free(cmd->out);
	cmd->out = NULL;
}

/*
** Libère une commande complète, y compris ses arguments et redirections.
*/
void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			cmd->args[i] = NULL;
			i++;
		}
		free(cmd->args);
		cmd->args = NULL;
	}
	free_input_redirections(cmd);
	free_output_redirections(cmd);
	free(cmd);
}

int	skip_spaces(const char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i);
}

int	is_empty_or_spaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}
