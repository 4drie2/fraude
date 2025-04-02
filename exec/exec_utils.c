/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 05:40:35 by vboxuser          #+#    #+#             */
/*   Updated: 2025/01/15 13:01:53 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_pipes(int pipefds[][2], t_state *state)
{
	if (state->index > 0)
	{
		if (dup2(pipefds[state->index - 1][0], STDIN_FILENO) == -1)
			exit(1);
	}
	if (state->index < state->num_pipes)
	{
		if (dup2(pipefds[state->index][1], STDOUT_FILENO) == -1)
			exit(1);
	}
	close_pipes(pipefds, state->num_pipes);
}

void	close_pipes(int pipefds[][2], int num_pipes)
{
	int	i;

	i = -1;
	while (++i < num_pipes)
	{
		close(pipefds[i][0]);
		close(pipefds[i][1]);
	}
}

void	print_error_and_exit(char *cmd, char *path, char *msg, int exit_code)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(msg, 2);
	exit(exit_code);
}

int	print_error(char *cmd, char *path, char *msg, int exit_code)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(msg, 2);
	return (exit_code);
}

/*
** Valide si le chemin fourni est un fichier exécutable.
** Si valide, retourne une copie du chemin.
** Sinon, affiche une erreur et quitte le programme.
*/
char	*validate_path(char *cmd_path)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0)
	{
		if (access(cmd_path, X_OK) != 0)
			print_error("", cmd_path, ": Permission denied\n", 126);
		return (ft_strdup(cmd_path));
	}
	print_error("", cmd_path, ": No such file or directory\n", 127);
	return (NULL);
}
