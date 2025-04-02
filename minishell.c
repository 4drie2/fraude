/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 21:23:50 by marvin            #+#    #+#             */
/*   Updated: 2024/12/11 21:23:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** handle_user_input:
**   - Compare à "exit"
**   - Parse l'input -> t_command
**   - Exécute la commande
*/
void	free_environment(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	handle_user_input(char *input, t_state *state)
{
	t_command	*cmd;

	if (!input || ft_strcmp(input, "exit") == 0)
	{
		ft_putstr_fd("exit\n", 1);
		free_environment(state->envp);
		free(input);
		exit(EXIT_SUCCESS);
	}
	if (is_empty_or_spaces(input))
		return (0);
	add_history(input);
	cmd = parse_input(input, state);
	if (!cmd)
		return (1);
	exec_shell(cmd, state);
	free_command_list(cmd);
	return (0);
}

/*
** display_prompt:
**   - Boucle infinie, lit l'input, appelle handle_user_input
*/
void	display_prompt(t_state *state)
{
	char	*input;

	while (1)
	{
		input = readline("\001\033[1;32m\002ᴍᴀᴇꜱᴛʀᴏꜱʜᴇʟʟ > \001\033[0m\002");
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		reset_state(state);
		if (*input)
			handle_user_input(input, state);
		free(input);
	}
}

/*
** init_state: initialise l'environnement, le chemin, etc.
*/
int	init_state(t_state *state, char **envp)
{
	char	*shlvl;
	char	*value;
	int		i;

	state->envp = copy_environment(envp);
	if (!state->envp)
		return (EXIT_FAILURE);
	shlvl = search_in_local_env(state->envp, "SHLVL");
	i = ft_atoi(shlvl) + 1;
	value = ft_itoa(i);
	state->envp = set_env_var(state->envp, "SHLVL", value);
	state->last_exit_status = 0;
	state->num_pipes = 0;
	state->index = 0;
	state->pids = NULL;
	state->pipes = NULL;
	state->o_stdout = -1;
	state->o_stdin = -1;
	if (getcwd(state->current_path, sizeof(state->current_path)) == NULL)
	{
		perror("Initialization: getcwd error");
		ft_strlcpy(state->current_path, "", 1);
		return (free(value), free_environment(state->envp), EXIT_FAILURE);
	}
	return (free(value), EXIT_SUCCESS);
}

/*
** main: init l'état, setup_signals, lance la boucle du prompt
*/
int	main(int argc, char **argv, char **envp)
{
	t_state	shell_state;

	(void) argc, (void) argv;
	if (init_state(&shell_state, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	setup_signals();
	display_prompt(&shell_state);
	rl_clear_history();
	free_environment(shell_state.envp);
	return (shell_state.last_exit_status);
}
