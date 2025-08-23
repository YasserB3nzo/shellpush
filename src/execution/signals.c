/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:42:34 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/22 15:08:06 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Handle a signal in child context used before exec to redraw prompt on SIGINT */
void	shell_signal_redraw(int signal)
{
	if (signal == SIGINT)
	{
		{
			printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			g_signal.ret = 130;
		}
	}
}

/* Restore default SIGINT handling (used in specific child scenarios) */
void	set_sigint_default(int s)
{
	(void)s;
	signal(SIGINT, SIG_DFL);
	write(1, "\n", 1);
}

/* Interactive SIGINT handler for main shell loop */
void	shell_sigint_handler(int sig)
{
	if (g_signal.ff == 1)
		return ;
	(void)sig;
	printf("\n");
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	g_signal.ret = 130;
}

/* Install main shell signal handlers */
void	install_shell_signal_handlers(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, shell_sigint_handler);
	signal(SIGTSTP, SIG_IGN);
}
