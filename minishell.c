/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:44:36 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/21 15:44:41 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

t_signal	g_signal;

int	data_init(char **av, t_data *data, char **env)
{
	(void)av;
	g_signal.ret = 0;
	g_signal.ret_exit = 0;
	data->path_flag = false;
	g_signal.ff = 0;
	if (!env[0])
		data->path_flag = true;
	data->list_env = copy_environment(env);
	return (1);
}

static void	cleanup_and_exit(t_data *data)
{
	if (data->env)
		free_array(data->env);
	senv_clear(&data->list_env);
	ft_putstr_fd("exit\n", 2);
}

static void	process_line(t_data *data)
{
	if (is_spaces(data->line) == 0)
	{
		add_history(data->line);
		g_signal.ret = 0;
		g_signal.ret_exit = 0;
	}
	else if (check_quotation(data->line) != -1)
	{
		add_history(data->line);
		parsing(data, NULL, NULL, -1);
		g_signal.ret_exit = g_signal.ret;
	}
	else
	{
		g_signal.ret = 2;
		g_signal.ret_exit = 2;
	}
}

static void	shell_loop(t_data *data)
{
	while (1)
	{
		signal_handler();
		data->env = linked_list_to_array(data->list_env);
		data->line = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
		if (!data->line)
			break ;
		process_line(data);
		free(data->line);
		free_array(data->env);
		g_signal.ff = 0;
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	if (ac != 1 || data_init(av, &data, env) == 0)
		return (1);
	shell_loop(&data);
	cleanup_and_exit(&data);
	return (g_signal.ret_exit);
}
