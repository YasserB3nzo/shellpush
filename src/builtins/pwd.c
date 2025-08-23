/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:58:26 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/23 18:26:57 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_oldpwd_change(t_env *env_list, char *oldpwd_val)
{
	char	*oldpwd_dup;
	char	buffer[PATH_MAX];
	char	*current_pwd;

	oldpwd_dup = ft_strdup(oldpwd_val);
	if (!oldpwd_dup)
		return ;
	current_pwd = getcwd(buffer, PATH_MAX);
	if (chdir(oldpwd_dup) == 0)
	{
		set_env_after_cd(env_list, "OLDPWD", current_pwd);
		set_env_after_cd(env_list, "PWD", oldpwd_dup);
		ft_putstr_fd(oldpwd_dup, 1);
		ft_putchar_fd('\n', 1);
	}
	else
		perror("cd");
	free(oldpwd_dup);
}

static void	handle_home_directory(t_env *env_list, char **command_args)
{
	char	*home_path;

	home_path = find_variable_value(env_list, env_list, "HOME", false);
	if (command_args[1] == NULL || command_args[1][0] == '~')
	{
		if (!home_path)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		else
			change_directory_path(env_list, home_path);
	}
}

static void	handle_oldpwd_directory(t_env *env_list)
{
	char	*oldpwd_val;

	oldpwd_val = find_variable_value(env_list, env_list, "OLDPWD", false);
	if (!oldpwd_val)
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	else
		handle_oldpwd_change(env_list, oldpwd_val);
}

void	change_directory(t_env *env_list, char **command_args)
{
	if (array_size(command_args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	if (command_args[1] == NULL || command_args[1][0] == '~')
		handle_home_directory(env_list, command_args);
	else if (command_args[1][0] == '-' && command_args[1][1] == '\0')
		handle_oldpwd_directory(env_list);
	else
		change_directory_path(env_list, command_args[1]);
}

void	print_working_directory(t_env *env_list)
{
	char	buffer[PATH_MAX];
	char	*current_path;
	char	*pwd_value;

	current_path = getcwd(buffer, PATH_MAX);
	if (current_path)
	{
		ft_putstr_fd(current_path, 1);
		ft_putchar_fd('\n', 1);
	}
	else
	{
		pwd_value = find_variable_value(env_list, env_list, "PWD", false);
		ft_putstr_fd(pwd_value, 1);
		ft_putchar_fd('\n', 1);
	}
}
