/* Simple variable expansion function that works like heredoc */
char	*simple_expand_variable(char *str, char **env)
{
	char	*result;
	char	*var_start;
	char	*var_end;
	char	*var_name;
	char	*var_value;
	int		i;
	int		result_len;
	int		result_pos;

	if (!str || !env)
		return (str);
	
	result_len = ft_strlen(str) + 1000; // Give extra space for expansion
	result = malloc(result_len);
	if (!result)
		return (str);
	
	result_pos = 0;
	i = 0;
	
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'))
		{
			// Found a variable
			var_start = &str[i + 1];
			var_end = var_start;
			
			if (*var_start == '?')
			{
				var_end++;
				var_name = ft_strdup("?");
			}
			else
			{
				while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
					var_end++;
				var_name = ft_substr(var_start, 0, var_end - var_start);
			}
			
			// Get variable value
			if (ft_strcmp(var_name, "?") == 0)
				var_value = ft_itoa(g_signal.ret);
			else
				var_value = get_env_variable_value(env, var_name);
			
			// Copy variable value to result
			if (var_value)
			{
				int j = 0;
				while (var_value[j] && result_pos < result_len - 1)
					result[result_pos++] = var_value[j++];
				free(var_value);
			}
			
			free(var_name);
			i = var_end - str; // Move past the variable
		}
		else
		{
			// Regular character, copy it
			if (result_pos < result_len - 1)
				result[result_pos++] = str[i];
			i++;
		}
	}
	
	result[result_pos] = '\0';
	return (result);
}
