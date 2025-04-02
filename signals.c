/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdumay <jdumay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 06:57:01 by vboxuser          #+#    #+#             */
/*   Updated: 2025/01/22 20:41:54 by jdumay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** handle_sigint:
**   - Intercepte le signal Ctrl-C (SIGINT)
**   - Affiche un saut de ligne,
**   - Vide l’éventuelle commande en cours de saisie,
**   - Redisplay le prompt.
*/
void	reset_signals_after_execution(void)
{
	setup_signals();
}

void	setup_signals_for_heredoc(void)
{
	struct sigaction	sa_int;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_int, NULL);
}

void	setup_signals_for_execution(void)
{
	struct sigaction	sa_default;

	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = SA_RESTART;
	sa_default.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}

void	handle_sigint(int signo)
{
	(void)signo;
	if (waitpid(-1, NULL, WNOHANG) == 0)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*
** handle_sigquit:
**   - Intercepte le signal Ctrl-\ (SIGQUIT)
**   - Ici, on choisit de ne rien afficher,
**     ou juste effacer le ^\ (historique ou autre).
*/

/*
** setup_signals:
**   - Installe les handlers pour SIGINT, SIGQUIT,
**   - Ignore SIGPIPE pour éviter les messages "Broken pipe".
*/
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
