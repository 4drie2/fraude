/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:56:30 by david             #+#    #+#             */
/*   Updated: 2025/01/22 15:05:58 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_numeric(const char *str, long *value)
{
	char	*endptr;

	if (!str || !*str)
		return (0);
	errno = 0;
	*value = ft_strtol(str, &endptr, 10);
	if (errno == ERANGE || *value > LLONG_MAX || *value < LLONG_MIN)
		return (0);
	while (*endptr && ft_isspace(*endptr))
		endptr++;
	if (*endptr != '\0')
		return (0);
	return (1);
}

//void	handle_exit(t_command *cmd, t_state *state)
//{
//	long	exit_code;
//
//	if (!cmd->args[1])
//	{
//		while (cmd->prev)
//			cmd = cmd->prev;
//		free_command_list(cmd);
//		free_environment(state->envp);
//		exit(state->last_exit_status = 0);
//	}
//	if (!check_numeric(cmd->args[1], &exit_code))
//	{
//		print_error("exit: ",
//			cmd->args[1], ": numeric argument required\n", 2);
//		state->last_exit_status = 2;
//		return ;
//	}
//	if (cmd->args[2])
//	{
//		print_error("exit: ", "", "too many arguments\n", 1);
//		state->last_exit_status = 2;
//		return ;
//	}
//	if (state->o_stdin != -1)
//		close(state->o_stdin);
//	if (state->o_stdout != -1)
//		close(state->o_stdout);
//	while (cmd->prev)
//		cmd = cmd->prev;
//	free_command_list(cmd);
//	free_environment(state->envp);
//	exit((unsigned char)exit_code);
//}

static void	handle_exit_no_args(t_command *cmd, t_state *st)
{
	while (cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	free_environment(st->envp);
	exit(st->last_exit_status = 0);
}

static void	handle_exit_numeric_err(t_state *st, char *arg)
{
	print_error("exit: ", arg, ": numeric argument required\n", 2);
	st->last_exit_status = 2;
}

static void	handle_exit_close_and_exit(t_command *cmd, t_state *st, long code)
{
	if (st->o_stdin != -1)
		close(st->o_stdin);
	if (st->o_stdout != -1)
		close(st->o_stdout);
	while (cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	free_environment(st->envp);
	exit((unsigned char)code);
}

void	handle_exit(t_command *cmd, t_state *st)
{
	long	exit_code;

	if (!cmd->args[1])
	{
		handle_exit_no_args(cmd, st);
		return ;
	}
	if (!check_numeric(cmd->args[1], &exit_code))
	{
		handle_exit_numeric_err(st, cmd->args[1]);
		return ;
	}
	if (cmd->args[2])
	{
		print_error("exit: ", "", "too many arguments\n", 1);
		st->last_exit_status = 2;
		return ;
	}
	handle_exit_close_and_exit(cmd, st, exit_code);
}
