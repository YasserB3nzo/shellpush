/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util16.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 02:10:30 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/24 22:39:29 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

static char	*join_path(char *dir, char *cmd)
{
	char	*path;

	path = ft_strjoin3(dir, '/', cmd);
	if (!path)
		return (NULL);
	if (access(path, X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

static char	*check_direct_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full;

	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (full)
			return (full);
		i++;
	}
	return (NULL);
}

char	*get_my_path(t_env *list, char **command_args, bool flag, int i)
{
	char	*env_path;
	char	**paths;
	char	*full;

	(void)i;
	if (!command_args || !command_args[0])
		return (NULL);
	full = check_direct_path(command_args[0]);
	if (full)
		return (full);
	env_path = find_variable_value(list, list, "PATH", flag);
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	full = search_in_paths(paths, command_args[0]);
	if (full)
	{
		free_array(paths);
		return (full);
	}
	free_array(paths);
	return (NULL);
}
