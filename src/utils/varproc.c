/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varproc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:36:49 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/22 15:11:04 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_variable_expansion(char *cmd, char **vars, t_line *data)
{
	data->pos = copy_string_to_buffer(data->line, vars[data->i++], data->pos);
	data->size++;
	if (cmd[data->size] == '?')
		data->size++;
	else
		while (cmd[data->size] && ft_isalnum(cmd[data->size]))
			data->size++;
}
