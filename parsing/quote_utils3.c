/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 03:44:14 by david             #+#    #+#             */
/*   Updated: 2025/01/15 03:45:31 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Fonction pour gérer les variables dans les quotes simples (pas d'expansion)
char	*handle_single_quotes(const char *input, int *i)
{
	int	start;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
		(*i)++;
	return (ft_substr(input, start, *i - start - 1));
}

char	*parse_quoted_token(const char *input,
	int *i, t_state *state)
{
	if (input[*i] == '\'')
		return (handle_single_quotes(input, i));
	else if (input[*i] == '"')
		return (handle_double_quotes(input, i, state));
	return (NULL);
}
