/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:45:01 by david             #+#    #+#             */
/*   Updated: 2025/01/15 07:34:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
** Vérifie si le chemin est un répertoire.
** Si c'est un répertoire, affiche une erreur et met g_exit_status.
** Retourne 0 en cas d'erreur, 1 sinon.
*/
static int	check_if_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_error("", path, ": Is a directory\n", 126);
		return (0);
	}
	return (1);
}

/*
** Ouvre un fichier en lecture seule pour les redirections d'entrée.
** Si l'ouverture réussit, duplique le descripteur vers STDIN.
** Retourne 0 en cas de succès, -1 en cas d'erreur.
*/
static int	handle_input_redirection(t_redir *redir, t_state *state)
{
	int	fd;

	if (!check_if_directory(redir->target))
		return (-1);
	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
		return (handle_access_error(redir->target, state));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
** Gère toutes les redirections d'entrée du command.
** Retourne 0 en cas de succès, -1 en cas d'erreur.
*/
static int	process_input_redirections(t_command *cmd, t_state *state)
{
	int	i;

	i = 0;
	while (i < cmd->nb_in)
	{
		if (handle_input_redirection(cmd->in[i], state) == -1)
			return (-1);
		i++;
	}
	return (0);
}

/*
** Gère toutes les redirections de sortie du command.
** Retourne 0 en cas de succès, -1 en cas d'erreur.
*/
static int	process_output_redirections(t_command *cmd, t_state *state)
{
	int	i;

	i = 0;
	while (i < cmd->nb_out)
	{
		if (handle_output_redirection(cmd->out[i], state) == -1)
			return (-1);
		i++;
	}
	return (0);
}

/*
** Fonction principale qui gère toutes les redirections
** (entrée et sortie) d'une commande.
** Retourne 0 en cas de succès, -1 en cas d'erreur.
*/
int	handle_all_redirections(t_command *cmd, t_state *state)
{
	if (process_input_redirections(cmd, state) == -1)
		return (-1);
	if (process_output_redirections(cmd, state) == -1)
		return (-1);
	return (0);
}
