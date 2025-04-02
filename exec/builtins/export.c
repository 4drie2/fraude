/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:08:27 by marvin            #+#    #+#             */
/*   Updated: 2024/12/14 21:08:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	if (*str == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
		{
			i++;
			break ;
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (i > 0 && str[i - 1] == '-')
		return (0);
	return (1);
}

static int	get_env_length(char **envp)
{
	int	len;

	len = 0;
	if (!envp)
		return (0);
	while (envp[len])
		len++;
	return (len);
}

/*
** Trie le tableau 'env' en ordre alphabétique (bubble sort).
*/
static void	sort_env_array(char **env, int len)
{
	int	i;
	int	j;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - 1 - i)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
				ft_swap_strings(&env[j], &env[j + 1]);
			j++;
		}
		i++;
	}
}

/*
** Duplique et trie l'environnement initial.
** Retourne le tableau trié ou NULL en cas d'erreur.
*/
char	**get_sorted_env(char **envp)
{
	char	**sorted;
	int		len;

	len = get_env_length(envp);
	sorted = ft_arraydup(envp);
	if (!sorted)
		return (NULL);
	sort_env_array(sorted, len);
	return (sorted);
}

int	has_leading_space(const char *str)
{
	if (ft_isspace(str[0]))
		return (1);
	return (0);
}
