/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:11:43 by david             #+#    #+#             */
/*   Updated: 2025/01/15 10:04:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	convert_tokens_to_commands(t_command **cmd, t_token *tokens, t_state *state)
{
	if (convert_tokens_to_command(*cmd, tokens, state) == -1)
	{
		free_command_list(*cmd);
		return (-1);
	}
	return (0);
}

/*
** Vérifie si l'entrée est valide (non NULL et non vide).
** Retourne 1 si valide, 0 sinon.
*/
int	validate_input(char *input)
{
	if (!input || !*input)
		return (0);
	return (1);
}

/*
** Tokenise l'entrée et vérifie la syntaxe des tokens.
** Retourne la liste des tokens si réussi, NULL sinon.
*/
t_token	*tokenize_and_validate(char *input, t_state *state)
{
	t_token	*tokens;

	tokens = tokenize_input(input, state);
	if (!tokens)
	{
		state->last_exit_status = 2;
		return (NULL);
	}
	if (check_syntax_errors(tokens, state))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
