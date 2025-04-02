/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 22:23:37 by vboxuser          #+#    #+#             */
/*   Updated: 2025/01/15 03:27:38 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_infile(t_command *cmd, t_token *current, t_state *state)
{
	t_token	*next;

	next = current->next;
	if (!next || !next->content)
	{
		ft_putstr_fd("syntax error near unexpected token 'newline'\n", 2);
		state->last_exit_status = 2;
		return (-1);
	}
	while (next && next->content && ft_isspace(*next->content))
		next = next->next;
	if (next && (check_redirections(current, state) == 0))
	{
		print_error(
			"syntax error near unexpected token '", next->content, "'\n", 2);
		return (-1);
	}
	return (add_in_redir(cmd, R_INPUT, next->content));
}

int	handle_outfile(t_command *cmd, t_token *current, int append)
{
	t_token	*next;

	next = current->next;
	if (!next || !next->content)
	{
		ft_putstr_fd("syntax error near unexpected token 'newline'\n", 2);
		return (2);
	}
	if (append)
		return (add_out_redir(cmd, R_APPEND, next->content));
	else
		return (add_out_redir(cmd, R_OUTPUT, next->content));
}

int	handle_heredoc_redirection(t_command *cmd, t_token *current)
{
	t_token	*next;
	int		ret;

	next = current->next;
	if (!next || !next->content)
	{
		ft_putstr_fd("syntax error near unexpected token 'newline'\n", 2);
		return (2);
	}
	ret = add_in_redir(cmd, R_HEREDOC, next->content);
	if (ret < 0)
		return (ret);
	return (1);
}

int	handle_redirection_type(t_command *cmd, const char *content, char *path)
{
	if (ft_strcmp(content, "<") == 0)
		return (add_in_redir(cmd, R_INPUT, path));
	else if (ft_strcmp(content, "<<") == 0)
		return (add_in_redir(cmd, R_HEREDOC, path));
	else if (ft_strcmp(content, ">") == 0)
		return (add_out_redir(cmd, R_OUTPUT, path));
	else if (ft_strcmp(content, ">>") == 0)
		return (add_out_redir(cmd, R_APPEND, path));
	return (-1);
}

int	handle_redirection(t_command *cmd, t_token *current, t_state *state)
{
	t_token	*next;
	char	*path;

	next = current->next;
	if (!next || !next->content)
	{
		print_error("syntax error near unexpected token '",
			"newline", "'\n", 2);
		return (-1);
	}
	if (check_redirections(current, state) == 0)
	{
		print_error("syntax error near unexpected token '",
			next->content, "'\n", 2);
		return (-1);
	}
	path = ft_strdup(next->content);
	if (!path)
		return (-1);
	return (handle_redirection_type(cmd, current->content, path));
	free(path);
	return (-1);
}
