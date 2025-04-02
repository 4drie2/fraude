/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 00:55:58 by marvin            #+#    #+#             */
/*   Updated: 2025/01/14 13:07:35 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_pwd_command(t_state *state)
{
	char	current_path[1024];

	if (ft_strlen(state->current_path) > 0)
		ft_printf("%s\n", state->current_path);
	else
	{
		if (getcwd(current_path, sizeof(current_path)) != NULL)
		{
			ft_printf("%s\n", current_path);
			ft_strlcpy(state->current_path, current_path,
				ft_strlen(current_path) + 1);
		}
		else
			perror("pwd: error getting current working directory");
	}
}
