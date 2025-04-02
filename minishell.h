/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 21:24:09 by marvin            #+#    #+#             */
/*   Updated: 2024/12/11 21:24:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>
# include <limits.h> 
# include <errno.h>

typedef struct s_heredoc
{
	char	*tmp_filename;
	char	*limiter;
	int		tmp_fd;
	char	**out_tmp_filename;
}	t_heredoc;

typedef struct s_Shell_State
{
	char		current_path[1024];
	char		**envp;
	int			last_exit_status;
	int			num_pipes;
	int			index;
	int			o_stdout;
	int			o_stdin;
	int			*pipes;
	pid_t		*pids;
	t_heredoc	*hd;
}	t_state;

typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
}	t_redir;

typedef struct s_command
{
	char				**args;

	t_redir				**in;
	int					nb_in;
	t_redir				**out;
	int					nb_out;
	char				*tmp_filename;
	int					tmp_fd;
	char				*path;

	int					is_pipe;
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;

typedef struct s_context
{
	t_command	*cmd;
	t_state		*state;
	int			exit;
}	t_context;

// Structure pour un token en liste chaînée
typedef struct s_token
{
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_fork_info
{
	int		(*pipes)[2];
	pid_t	*pids;
	int		num_cmds;
}	t_fork_info;

/*
** Exemple de structure pour stocker infos de heredoc :
** tmp_filename : "/tmp/heredoc_1234"
** tmp_fd       : FD si besoin
** limiter      : la chaîne "EOF" par exemple
*/

/*
** Exit Status Codes
** 0   - Success
** 1   - General errors
** 2   - Syntax errors
** 126 - Command found but not executable
** 127 - Command not found
** 130 - Script terminated by Control-C
*/

t_token		*tokenize_and_validate(char *input, t_state *state);
int			is_redirection(const char *content);
int			is_file_tok(t_token *file_tok, t_state *state);
void		print_err_msg(char *cmd, char *path, char *msg);
void		handle_empty_cmd(t_command *cmd, t_state *st);
void		child_process(t_command *cmd, t_state *st,
				t_fork_info *fi, int i);
int			handle_single_command(t_command **cmd,
				t_state *st, t_fork_info *fi, int i);

// Fonctions d'exécution
void		close_child_fds(t_state *st);
void		init_child_context(t_command *cmd, t_state *st);
void		process_heredocs_or_fail(t_command *cmd, t_state *st, char *path);
void		process_redirections_or_fail(t_command *cmd, t_state *st);
void		execute_binary_or_fail(t_command *cmd, t_state *st, char *path);
int			exec_if_builtin(t_command *cmd, t_state *st);
int			check_if_empty_command(t_command *cmd, t_state *st);
int			check_dir_and_exec(char *cmd, t_state *st, bool *check);
int			check_file_existence(char *cmd, t_state *st, bool *check);
void		cleanup_command(t_command *cmd);
void		cleanup_heredoc(t_heredoc *hd);
void		cleanup_state(t_state *st);
void		handle_redirections_or_exit(t_command *cmd, t_state *st);
void		copy_environment_or_exit(t_state *st,
				t_state *temp_st, char ***temp_env);
void		handle_no_args_or_exit(t_command *cmd,
				t_state *st, char **temp_env);
void		handle_builtin_or_exit(t_command *cmd,
				t_state *temp_st, char **temp_env);
void		execute_builtin(t_command *cmd, t_state *temp_state);
void		setup_pipes(int pipefds[][2], t_state *state);
void		execute_external(t_command *cmd, t_state *state);
int			is_builtin(char *cmd);
void		close_pipes(int pipefds[][2], int num_pipes);

void		handle_cd_command(char **argv, t_state *state);
void		handle_pwd_command(t_state *state);
void		handle_env_command(char **envp, t_state *state);
void		handle_echo_command(t_command *cmd, t_state *state);
void		handle_unset_command(char **argv, char **envp);
void		handle_export_command(char **argv, t_state *state);
void		handle_neosnitch_command(char **argv, t_state *state);
void		handle_exit(t_command *cmd, t_state *state);

void		exec_shell(t_command *cmd, t_state *state);
void		unlink_all_heredocs(t_command *cmd);
void		handle_no_cmd(t_command *cmd, t_state *state);
void		handle_external_cmd(t_command *cmd, char **argv, t_state *state);
void		handle_multiples_pipes(t_command *cmd, t_state *state);
int			exec_builtins(t_command *cmd, t_state *state);
char		*join_key_value(const char *key, const char *value);
void		replace_env_entry(char **envp, int index, const char *key,
				const char *value);
char		*get_command_path(char *cmd, t_state *state);
void		reset_state(t_state *state);

char		**copy_environment(char **envp);
char		**set_env_var(char **envp, const char *key, const char *value);
char		**realloc_tab(char **envp, char *new_entry);
int			handle_all_redirections(t_command *cmd, t_state *state);
int			add_in_redir(t_command *cmd, t_redir_type type,
				const char *content);
int			add_out_redir(t_command *cmd, t_redir_type type,
				const char *content);
char		**ft_arraydup(char **array);

int			check_syntax(t_command *start_cmd, t_state *state);
int			count_commands(t_command *start_cmd);
int			allocate_pipes_pids(int num_cmds, int **pipes,
				pid_t **pids, t_state *state);
int			create_pipes(int num_pipes, int pipes[][2]);
int			fork_and_execute(t_command *cmd, t_state *state,
				int pipes[][2], pid_t *pids);
void		close_all_pipes(int num_pipes, int pipes[][2]);
void		wait_children(int num_cmds, pid_t *pids, t_state *state);
int			handle_output_redirection(t_redir *redir, t_state *state);
int			handle_access_error(char *target, t_state *state);
char		*get_cd_path(char **argv, t_state *state);
void		update_env_vars(t_state *state);
int			check_cd_path_exists(char *path);
int			check_cd_path_dir(char *path);
int			check_cd_args(char **argv);
char		**get_sorted_env(char **envp);
int			is_valid_identifier(const char *str);
char		*validate_path(char *cmd_path);
void		execute_command(t_command *cmd, t_state *state);
int			has_equals_as(char *str, char *ui);
int			print_error(char *cmd, char *path, char *msg, int exit_code);
void		print_error_and_exit(char *cmd, char *path,
				char *msg, int exit_code);
void		handle_source_command(char **args, t_state *state);
void		handle_command_error(t_state *state, char *cmd, int error_code);
int			is_path_absolute_or_relative(char *cmd);
int			validate_command_path(char *cmd, char **path,
				t_state *state, bool *check);
int			check_output_target(t_redir *redir,
				struct stat *st, t_state *state);
int			count_commands(t_command *start_cmd);
int			allocate_and_create_pipes(t_command *start_cmd,
				int **pipes, pid_t **pids, t_state *state);
void		execute_and_cleanup(t_command *start_cmd,
				t_state *state, int (*pipes)[2], pid_t *pids);

// Parsing

int			validate_input(char *input);
t_command	*initialize_command(t_state *state);
int			handle_commands(t_command **cmd, t_token *tokens, t_state *state);
void		cleanup_on_error(t_token *tokens, t_command *cmd,
				int error_code, t_state *state);
int			check_syntax_errors(t_token *tokens, t_state *state);
char		*handle_quotes(const char *input, int *i,
				char **envp, t_state *state);
char		*handle_variable(const char *input, int *i, t_state *state);
char		*handle_normal_char(const char *input, int *i);
int			append_to_word(char **word, char *temp);
int			process_current_char(const char *input, int *i,
				char **word, t_state *state);
int			check_initial_pipe(t_token *current, t_state *state);
int			check_pipes(t_token *current, t_state *state);
int			check_redirections(t_token *current, t_state *state);
int			check_invalid_tokens(t_token *current, t_state *state);
t_token		*create_and_add_token(char *content, t_token **head);
t_command	*parse_input(char *input, t_state *state);
t_command	*init_command(void);
int			handle_redirections(t_command *cmd);
int			handle_token(t_token **head, const char *input,
				int *i, t_state *state);
t_token		*tokenize_input(const char *input, t_state *state);
int			skip_spaces(const char *input, int i);
char		*parse_quoted_token(const char *input, int *i, t_state *state);
char		*expand_variable(char *input, int *i, t_state *state);
int			check_unclosed_quotes(const char *input);
int			handle_all_heredocs(t_command *cmd, t_state *state);
int			handle_redirection(t_command *current_cmd,
				t_token *current, t_state *state);
char		*parse_single_quote(const char *input, int *i);
char		*parse_double_quote(const char *input, int *i,
				char **envp, t_state *state);
int			is_special_char(char c);
int			handle_arguments(t_command *current_cmd,
				t_token *current, int *arg_index);
int			handle_pipe(t_command **current_cmd,
				int *arg_index, t_state *state);
int			fill_command_args(t_command *cmd, t_token *current, int *index);
int			is_command_incomplete(t_token *tokens, t_state *state);
int			add_in_redir(t_command *cmd, t_redir_type type,
				const char *content);
int			add_out_redir(t_command *cmd,
				t_redir_type type, const char *content);
int			convert_tokens_to_command(t_command *cmd,
				t_token *tokens, t_state *state);
void		append_tokens(t_token **current, t_token *new_tokens);
int			is_empty_or_spaces(const char *str);
char		*search_in_local_env(char **envp, const char *key);
char		*handle_special_var(char c, t_state *state);
int			convert_tokens_to_commands(t_command **cmd,
				t_token *tokens, t_state *state);
int			should_parse_as_special(const char *input, int i);
int			is_empty_or_spaces(const char *str);
int			has_leading_space(const char *str);
char		*normalize_spaces(const char *str);
int			is_file_tok(t_token *file_tok, t_state *state);
int			is_redirection(const char *content);
int			process_tokenizer(t_command **curr,
				t_token **cur, int *arg_idx, t_state *state);
char		*handle_double_quotes(const char *input, int *i, t_state *state);
int			fork_child(t_heredoc *hd, t_command *cmd,
				t_state *st, struct sigaction *o);
int			fork_parent(pid_t p, t_heredoc *hd, t_state *st,
				struct sigaction *o);
char		*generate_tmp_filename(void);
void		child_read_heredoc(t_heredoc *hd,
				t_command *cmd, t_state *state);
void		free_child_and_exit(t_heredoc *hd, t_command *cmd, t_state *st);
void		read_and_write_heredoc(t_heredoc *hd);
int			open_heredoc_file(t_heredoc *hd);
void		init_signal_context(t_heredoc *hd, t_command *cmd, t_state *st);

// Signals
void		handle_sigint(int signo);
// void		handle_sigquit(int signo);
void		setup_signals(void);
void		setup_signals_for_execution(void);
void		reset_signals_after_execution(void);
void		setup_signals_for_heredoc(void);
void		child_cleanup_and_exit(int signal);
t_context	*get_signal_context(void);
void		restore_and_close_fds(t_state *state);
// Utils
void		free_command(t_command *cmd);
void		free_args(char **args);

//free
void		free_command_list(t_command *cmd);
void		free_tokens(t_token *head);
void		free_environment(char **envp);
void		free_redirections(t_command *cmd);

#endif
