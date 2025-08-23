/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:12:46 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/22 15:07:53 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Standard includes */
# include "../Libft/libft.h"
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

/* Forward declaration */
typedef struct s_data	t_data;

/* Signal handling structure */
typedef struct s_signal
{
	int					ret;
	int					ret_exit;
	int					sig;
	int					ff;
}						t_signal;

extern t_signal			g_signal;

/* Environment variable structure */
typedef struct s_env
{
	char				*var_name;
	struct s_env		*next;
	struct s_env		*prev;
}						t_env;

/* Token enumeration */
typedef enum s_token
{
	Cmd,
	AppendFile,
	HereDocDel,
	Infile,
	OutFile,
	Operation,
	NonOperation,
	Input,
	Output,
	Append,
	HereDoc,
	Pipe,
	Non
}						t_token;

/* Command structure */
typedef struct s_cmds
{
	char				**cmd;
	t_token				token;
	t_token				operation;
	t_data				*data;
	struct s_cmds		*next;
	struct s_cmds		*prev;
}						t_cmds;

/* Simple list structure */
typedef struct s_slist
{
	char				*cmd;
	t_token				token;
	struct s_slist		*next;
	struct s_slist		*prev;
}						t_slist;

/* Command execution structure */
typedef struct s_command
{
	char				**cmd;
	t_slist				*infile;
	t_slist				*outfile;
	struct s_command	*next;
	struct s_command	*prev;
}						t_command;

/* Main data structure */
struct					s_data
{
	int					fd[2];
	int					*pid;
	int					k;
	int					fd_in;
	char				**env;
	char				**cmds;
	char				*line;
	bool				path_flag;
	bool				flag;
	t_env				*list_env;
	t_cmds				*lst;
	t_command			*list;
};

/* Line processing structure */
typedef struct s_line
{
	char				*line;
	int					i;
	int					k;
	int					pos;
	int					size;
}						t_line;

/* ========================================================================== */
/*                             FUNCTION PROTOTYPES                           */
/* ========================================================================== */

/* List management functions */
t_cmds					*lstlast(t_cmds *command_list);
void					lstclear(t_cmds **command_list);
t_cmds					*lstnew(char *cmd, t_cmds *stack, char **str);
t_slist					*nodes_last(t_slist *node_list);
t_command				*command_last(t_command *command_list);
t_command				*command_new(t_command *lst);
void					commands_clear(t_command **command_list);
void					slist_clear(t_slist **lst);

/* Environment management functions */
t_env					*env_new(t_env *lst, char *str);
t_env					*env_last(t_env *lst);
t_env					*copy_environment(char **env);
t_env					*find_env_variable(t_env *list, char *variable_name);
t_env					*unset_env(t_env *list, char **command_args, t_data *data);
char					**linked_list_to_array(t_env *list);
char					*get_env_variable_value(char **env, char *variable_name);
char					*find_variable_value(t_env *list, t_env *head, char *variable_name, bool flag);
void					senv_clear(t_env **lst);
char					**create_environment(void);

/* String utilities */
char					*ft_strjoin3(char const *s1, char c, char const *s2);
char					*rm_spaces(char *str);
char					*ndup(const char *s, size_t n);
char					*get_string(char *str, size_t i, size_t k, size_t size);
char					*increment_s1(char *s1);
char					**ft_split_str(char *s1);
char					**array_copy(char **str);
void					free_array(char **array);
void					print_value(char *str);

/* Counting and size utilities */
int						array_size(char **var);
int						count_nodes(t_env *list);
int						count_words(char const *s, int count, int in_word);
int						get_cmd_size(t_cmds *list);
int						get_command_size(t_command *command);
int						calculate_total_string_length(char **vars, char **lines);

/* String checking utilities */
int						is_spaces(char *str);
int						check_quotation(char *str);
int						check_quot(char *str);
bool					check_n_flag(char *str);
bool					check_ex(char *str, int size);
bool					check_eq(char *str);
bool					check_next(char *str);
bool					check_next_for_both(char *str);
bool					check_back_for_heredoc(char *str, int index);
bool					is_it_inside(char *str);
int						is_numeric(char *str);

/* Variable expansion functions */
int						count_dollar_signs(char *str);
int						count_dollar_variables(char *s1);
int						how_many_dollar_in(char *str);
char					*extract_variable_name(char *line, int i, int j, int k);
char					*expand_variable(char *str, t_data *data);
char					*simple_expand_variable(char *str, char **env);
char					*check_expand(char *str, t_data *data);
char					**get_vars(char *cmd);
char					*get_final_line(char **lines, char **vars, char *cmd, t_line *line_data);
void					process_variable_expansion(char *cmd, char **vars, t_line *data);

/* Parsing functions */
void					get_list(char **cmd, int size, t_cmds **lst, t_data *data);
void					init_tokens(t_cmds *cmds, int token_size, t_cmds *lst);
void					parsing(t_data *data, t_cmds *lst, t_command *commands, int i);
void					non_token(t_cmds *lst);
void					remove_quotes(t_cmds *lst);
char					*get_cmd(char *cmd);
char const				*get_position(char const *s);
size_t					get_size(char *str);

/* Command processing functions */
t_command				*get_command(t_cmds *lst);
t_command				*get_commands(t_cmds *lst);
void					get_command_done(t_cmds *list, t_cmds *head, char **command, bool flag);
int						get_command_in_one_char(char **str);
char					**get_file_name(char *str);
char					**get_name(char *str);
char					**get_key_and_value(char *str, char **ptr, int i, int j);

/* List copying and merging */
t_cmds					*copy_node(char **cmd, t_token token, bool flag);
t_cmds					*copy_single_node(t_cmds *curr, int *i);
t_cmds					*add_head_to_new_list(t_cmds *head, t_cmds *new_head);
t_cmds					*copy_list(t_cmds *curr, char **command, int i);
t_cmds					*merge_lists(t_cmds *list1, t_cmds *list2);

/* Token and operation checking */
int						check_for_pipe(t_cmds *cmds);
int						check_for_in_out_put(t_cmds *cmds);
int						check_for_append_heredoc(t_cmds *cmds);
int						check_double(char *cmd, int i);

/* Error handling functions */
int						cmdcheck(char *str);
int						errormsg(char *str);
int						errormsg_v1(char *str);
int						errormsg_v2(char *str);
int						errors_managment(t_data *data, int flag);
void					shell_signal_redraw(int signal);
void					ft_puterror_fd(char *str1, char *str2);

/* Signal handling */
void					install_shell_signal_handlers(void);
void					shell_sigint_handler(int sig);

/* Execution functions */
int						executing(t_data *data);
int						execute_command(t_env *list, t_command *command, t_data *data, int index);
void					only_builtins(t_data *data, t_command *list, int builtin_command);
int						run_builtins(int c, t_command *command, t_data *data, int flag);
int						ft_fork(void);
void					ft_close(int fd, char *str);
int						wait_pid(int *pid, int cmd_num);

/* I/O redirection functions */
int						hand_the_redirectionin(t_command *lst);
int						open_heredoc(t_cmds *cmds, int pid, int status, bool flag);
int						ft_strcmp_for_heredoc(char *s1, char *s2);

/* Built-in command functions */
void					change_directory(t_env *list, char **command_args);
void					change_directory_path(t_env *list, char *path);
void					execute_echo_command(char **command_args, bool flag, int i);
void					export(t_env *list, char **command_args, char c, int i);
void					exit_myminishell(char **command_args, int flag);
void					print_working_directory(t_env *env);
void					print_environment(t_env *list);
int						print_export_variables(t_env *list);
char					*get_my_path(t_env *list, char **command_args, bool flag, int i);

/* Environment setting functions */
void					set_env_after_export(t_env *list, char **export, char c, bool export_flag);
void					set_env_after_cd(t_env *list, char *key, char *value);
void					set_env_if_plus(t_env *env_node, char *export_value);

/* Utility functions */
void					swap(char **s1, char **s2);
void					ft_clear(t_data *data);
int						copy_string_to_buffer(char *line, char *ptr, int pos);

#endif
