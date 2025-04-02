/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:48:24 by david             #+#    #+#             */
/*   Updated: 2025/01/22 14:54:53 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fork_parent(pid_t p, t_heredoc *hd, t_state *st,
				struct sigaction *o)
{
	int	s;

	waitpid(p, &s, 0);
	sigaction(SIGINT, o, NULL);
	if (st->pipes)
		close_all_pipes(st->num_pipes, (int (*)[2])st->pipes);
	if ((WIFSIGNALED(s) && WTERMSIG(s) == SIGINT)
		|| (WIFEXITED(s) && WEXITSTATUS(s) != 0))
	{
		unlink(hd->tmp_filename);
		free(hd->tmp_filename);
		return (-1);
	}
	*(hd->out_tmp_filename) = hd->tmp_filename;
	return (0);
}

int	fork_child(t_heredoc *hd, t_command *cmd,
				t_state *st, struct sigaction *o)
{
	sigaction(SIGINT, o, NULL);
	child_read_heredoc(hd, cmd, st);
	return (0);
}
