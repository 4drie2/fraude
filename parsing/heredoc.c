/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 23:23:11 by vboxuser          #+#    #+#             */
/*   Updated: 2025/01/22 16:06:32 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 
** genere un nom de fichier dans /tmp, style /tmp/heredoc_1234
** (pid = 1234 par ex.)
*/
char	*generate_tmp_filename(void)
{
	static int	counter;
	char		*filename;
	char		*pid_str;
	char		*count_str;
	char		*temp;

	counter = 0;
	pid_str = ft_itoa((int)getpid());
	if (!pid_str)
		return (NULL);
	count_str = ft_itoa(counter++);
	if (!count_str)
		return (free(pid_str), NULL);
	temp = ft_strjoin(pid_str, "_");
	free(pid_str);
	if (!temp)
		return (free(count_str), NULL);
	filename = ft_strjoin("/tmp/heredoc_", temp);
	free(temp);
	free(count_str);
	if (!filename)
		perror("Error generating temporary filename");
	return (filename);
}

/* 
** Ecrit ligne par ligne dans fd, jusqu'au limiter
*/
int	write_to_tmp_file(int fd, const char *limiter)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line | !ft_strcmp(line, limiter))
			break ;
		if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
		{
			perror("Error writing to temporary file");
			free(line);
			return (-1);
		}
		free(line);
	}
	free(line);
	return (0);
}

void	init_signal_context(t_heredoc *hd, t_command *cmd, t_state *st)
{
	t_context	*ctx;

	ctx = get_signal_context();
	ctx->cmd = cmd;
	ctx->state = st;
	st->hd = hd;
	signal(SIGINT, child_cleanup_and_exit);
	signal(SIGQUIT, SIG_DFL);
}

int	open_heredoc_file(t_heredoc *hd)
{
	hd->tmp_fd = open(hd->tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (hd->tmp_fd < 0)
	{
		perror("open heredoc file");
		return (-1);
	}
	return (0);
}

void	read_and_write_heredoc(t_heredoc *hd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, hd->limiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(hd->tmp_fd, line, ft_strlen(line)) < 0
			|| write(hd->tmp_fd, "\n", 1) < 0)
		{
			perror("write");
			free(line);
			return ;
		}
		free(line);
	}
}
