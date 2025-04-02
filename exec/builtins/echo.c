/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 22:43:22 by marvin            #+#    #+#             */
/*   Updated: 2024/12/14 22:43:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Fonction auxiliaire pour vérifier si une chaîne contient uniquement des 'n'
//static int	is_all_n(const char *str)
//{
//	int	i;
//
//	i = 0;
//	while (str[i])
//	{
//		if (str[i] != 'n')
//			return (0);
//		i++;
//	}
//	return (1);
//}

void	handle_echo_command(t_command *cmd, t_state *state)
{
	int		i;
	int		newline;

	if (!cmd->args[1])
		return (ft_putchar_fd('\n', STDOUT_FILENO));
	i = 1;
	newline = 1;
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 2) == 0
		&& has_equals_as((cmd->args[i] + 2), "n"))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	state->last_exit_status = 0;
}
