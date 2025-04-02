/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdumay <jdumay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:47:31 by david             #+#    #+#             */
/*   Updated: 2025/01/22 20:42:26 by jdumay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
** Obtient le chemin exécutable de la commande.
** Si la commande est absolue ou relative, valide le chemin.
** Sinon, recherche le chemin dans les PATHs.
** Si le chemin n'est pas trouvé, affiche une erreur et quitte.
*/
static char	*get_executable_path(t_command *cmd, t_state *state)
{
	char	*path;

	if (cmd->args[0][0] == '/' || (cmd->args[0][0] == '.'
		&& cmd->args[0][1] == '/'))
		path = validate_path(cmd->args[0]);
	else
		path = get_command_path(cmd->args[0], state);
	if (!path)
	{
		print_err_msg(cmd->args[0], ": command not found", "\n");
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_environment(state->envp);
		state->last_exit_status = 127;
		exit (127);
	}
	return (path);
}

/*
** Exécute la commande en utilisant execve.
** Si execve échoue, affiche une erreur et quitte le programme.
*/
static void	execute_cmd(char *path, char **args, char **envp)
{
	if (execve(path, args, envp) == -1)
	{
		perror(args[0]);
		free(envp);
		free(path);
		exit(126);
	}
}

/*
** Fonction principale pour exécuter une commande externe.
** Obtient le chemin exécutable et exécute la commande.
*/
void	execute_external(t_command *cmd, t_state *state)
{
	char	*path;

	path = get_executable_path(cmd, state);
	if (!path)
	{
		free_command_list(cmd);
		free_environment(state->envp);
		exit(state->last_exit_status);
	}
	execute_cmd(path, cmd->args, state->envp);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, ".") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "neosnitch") == 0)
		return (1);
	return (0);
}

int	check_output_target(t_redir *redir, struct stat *st, t_state *state)
{
	if (stat(redir->target, st) == 0 && S_ISDIR(st->st_mode))
	{
		ft_putstr_fd(redir->target, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		state->last_exit_status = 126;
		return (-1);
	}
	if (!redir->target || redir->target[0] == '\0')
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		state->last_exit_status = 2;
		return (-1);
	}
	return (0);
}
