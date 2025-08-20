/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:13:50 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/20 18:13:54 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*env_new(t_env *lst, char *str)
{
	t_env	*n_node;
	t_env	*last_node;

	n_node = (t_env *)malloc(sizeof(struct s_env));
	if (n_node == NULL)
		return (NULL);
	n_node->var_name = str;
	n_node->next = NULL;
	if (lst == NULL)
	{
		n_node->prev = NULL;
	}
	else
	{
		last_node = env_last(lst);
		n_node->prev = last_node;
	}
	return (n_node);
}

void	create_env_list(t_env **list, char **env, int subshell, int i)
{
	char	*shell_level_str;
	char	**env_parts;
	t_env	*node;
	t_env	*current;

	*list = env_new(*list, ft_strdup(env[i++]));
	while (env[i])
	{
		if (ft_strncmp("SHLVL", env[i], 4) == 0)
		{
			env_parts = ft_split(env[i++], '=');
			subshell = ft_atoi(env_parts[1]);
			shell_level_str = ft_itoa(++subshell);
			free_array(env_parts);
			node = env_new(*list, ft_strjoin3("SHLVL", '=', shell_level_str));
			free(shell_level_str);
		}
		else
			node = env_new(*list, ft_strdup(env[i++]));
		current = env_last(*list);
		current->next = node;
	}
}

t_env	*copy_environment(char **env)
{
	t_env	*env_list;

	env_list = NULL;
	if (!env[0])
	{
		env = create_environment();
		create_env_list(&env_list, env, 0, 0);
		free_array(env);
		return (env_list);
	}
	create_env_list(&env_list, env, 0, 0);
	return (env_list);
}

void	ft_putendle(char *str, int fd)
{
	if (!str)
		return ;
	ft_putstr_fd(str, fd);
	ft_putstr_fd("\n", fd);
}

void	print_environment(t_env *env_list)
{
	int		i;
	bool	has_equals;

	while (env_list)
	{
		i = 0;
		has_equals = false;
		while (env_list->var_name[i])
		{
			if (env_list->var_name[i] == '=')
			{
				has_equals = true;
				break ;
			}
			i++;
		}
		if (has_equals == true)
		{
			ft_putstr_fd(env_list->var_name, 1);
			ft_putchar_fd('\n', 1);
		}
		env_list = env_list->next;
	}
}
