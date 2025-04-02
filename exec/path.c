/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 07:27:30 by marvin            #+#    #+#             */
/*   Updated: 2025/01/15 07:27:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*try_paths(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*full;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (ft_free_array(paths), full);
		free(full);
		i++;
	}
	return (ft_free_array(paths), NULL);
}

char	*get_command_path(char *cmd, t_state *state)
{
	char		**paths;
	char		*path_env;
	char		*validated_path;
	struct stat	st;

	if (access(cmd, X_OK) == 0)
	{
		if (stat(cmd, &st) == 0 && !S_ISDIR(st.st_mode))
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_env = search_in_local_env(state->envp, "PATH");
	if (!path_env || path_env[0] == '\0')
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	validated_path = try_paths(paths, cmd);
	return (validated_path);
}

int	handle_access_error(char *target, t_state *state)
{
	if (errno == EACCES)
	{
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		state->last_exit_status = 1;
	}
	else
	{
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		state->last_exit_status = 1;
	}
	return (-1);
}

/*
** Vérifie si la commande utilise un chemin absolu ou relatif.
** Retourne 1 si oui, 0 sinon.
*/
int	is_path_absolute_or_relative(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (1);
	return (0);
}

/*
** Valide le chemin de la commande.
** Si valide, alloue et retourne une copie du chemin.
** Sinon, affiche une erreur, met à jour l'état et retourne 0.
*/
int	validate_command_path(char *cmd, char **path, t_state *st, bool *check)
{
	if (!check_file_existence(cmd, st, check))
		return (0);
	if (!check_dir_and_exec(cmd, st, check))
		return (0);
	*path = ft_strdup(cmd);
	if (!*path)
		return (0);
	return (1);
}
