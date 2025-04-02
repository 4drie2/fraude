/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:41:03 by david             #+#    #+#             */
/*   Updated: 2025/01/22 14:55:06 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fork_one_heredoc(const char *lim, char **out_tmp,
		t_command *cmd, t_state *st)
{
	t_heredoc			hd;
	pid_t				pid;
	struct sigaction	ign;
	struct sigaction	old;

	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	ign.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &ign, &old) < 0)
		return (-1);
	hd.tmp_filename = generate_tmp_filename();
	if (!hd.tmp_filename)
		return (-1);
	hd.limiter = (char *)lim;
	hd.out_tmp_filename = out_tmp;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(hd.tmp_filename);
		return (-1);
	}
	if (pid == 0)
		return (fork_child(&hd, cmd, st, &old));
	return (fork_parent(pid, &hd, st, &old));
}

/*
** Exemple d'une fonction handle_all_heredocs utilisant fork_one_heredoc
** Supposons que cmd->in[] soit un tableau de redirections,
** et R_HEREDOC indique qu'il faut lire un heredoc dont le limiter
** est dans cmd->in[i]->target.
*/
int	handle_all_heredocs(t_command *cmd, t_state *state)
{
	int		i;
	char	*tmpfile;

	i = 0;
	while (i < cmd->nb_in)
	{
		if (cmd->in[i]->type == R_HEREDOC)
		{
			tmpfile = NULL;
			if (fork_one_heredoc(cmd->in[i]->target, &tmpfile, cmd, state) < 0)
			{
				return (-1);
			}
			free(cmd->in[i]->target);
			cmd->in[i]->target = tmpfile;
		}
		i++;
	}
	return (0);
}

void	free_child_and_exit(t_heredoc *hd, t_command *cmd, t_state *st)
{
	close(hd->tmp_fd);
	if (cmd->is_pipe)
		close_all_pipes(st->num_pipes, (int (*)[2])st->pipes);
	if (st->pids)
		free(st->pids);
	if (st->pipes)
		free(st->pipes);
	if (cmd->path)
		free(cmd->path);
	while (cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	if (hd->tmp_filename)
		free(hd->tmp_filename);
	free_environment(st->envp);
	exit(0);
}

void	child_read_heredoc(t_heredoc *hd, t_command *cmd, t_state *st)
{
	init_signal_context(hd, cmd, st);
	if (open_heredoc_file(hd) < 0)
		exit(1);
	read_and_write_heredoc(hd);
	free_child_and_exit(hd, cmd, st);
	exit(0);
}
