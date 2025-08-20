/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:12:46 by ybenzidi          #+#    #+#             */
/*   Updated: 2025/08/20 18:12:55 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <dirent.h>
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
# include <time.h>

typedef struct s_data	t_data;

typedef struct s_signal
{
	int					ret;
	int					ret_exit;
	int					sig;
	int					ff;

}						t_signal;

extern t_signal			g_signal;

typedef struct s_env
{
	char				*var_name;

	struct s_env		*next;
	struct s_env		*prev;
}						t_env;

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

typedef struct s_cmds
{
	char				**cmd;

	t_token				token;
	t_token				operation;
	t_data				*data;

	struct s_cmds		*next;
	struct s_cmds		*prev;
}						t_cmds;

typedef struct s_slist
{
	char				*cmd;

	t_token				token;

	struct s_slist		*next;
	struct s_slist		*prev;
}						t_slist;

typedef struct s_command
{
	char				**cmd;

	t_slist				*infile;
	t_slist				*outfile;

	struct s_command	*next;
	struct s_command	*prev;
}						t_command;

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

typedef struct s_line
{
	char				*line;
	int					i;
	int					k;
	int					pos;
	int					size;
}						t_line;
t_cmds					*lstlast(t_cmds *command_list);
void					lstclear(t_cmds **command_list);
t_cmds					*lstnew(char *cmd, t_cmds *stack, char **str);
void					signal_handler(void);
char					*ft_strjoin3(char const *s1, char c, char const *s2);
void					free_array(char **array);
char					*rm_spaces(char *str);
void					get_list(char **cmd, int size, t_cmds **lst,
							t_data *data);
void					init_tokens(t_cmds *cmds, int token_size, t_cmds *lst);
void					parsing(t_data *data, t_cmds *lst, t_command *commands,
							int i);
char					*get_cmd(char *cmd);
int						errors_managment(t_data *data, int flag);
int						check_for_pipe(t_cmds *cmds);
int						cmdcheck(char *str);
int						errormsg(char *str);
int						errormsg_v1(char *str);
void					printsignalsc(int signal);
int						errormsg_v2(char *str);
int						is_spaces(char *str);
void					non_token(t_cmds *lst);
int						check_for_in_out_put(t_cmds *cmds);
int						check_for_append_heredoc(t_cmds *cmds);
int						errormsg(char *str);
int						check_quotation(char *str);
int						count_words(char const *s, int count, int in_word);
char					*ndup(const char *s, size_t n);
void					remove_quotes(t_cmds *lst);
int						dollar_is_in(char *str);
char					*grep_variable_name(char *line, int i, int j, int k);
char					**ft_split_str(char *s1);
char					*expand_variable(char *str, t_data *data);
char					**get_vars(char *cmd);
char					*get_final_line(char **lines, char **vars, char *cmd,
							t_line *line_data);
int						dollar_is_in(char *str);
int						count_vars(char *s1);
bool					check_ex(char *str, int size);
char					*check_expand(char *str, t_data *data);
char const				*get_position(char const *s);
size_t					get_size(char *str);
char					*get_string(char *str, size_t i, size_t k, size_t size);
int						run_builtins(int c, t_command *command, t_data *data,
							int flag);
int						hand_the_redirectionin(t_command *lst);
void					ft_handle_sigint(int sig);
t_slist					*nodes_last(t_slist *node_list);
t_command				*command_last(t_command *command_list);
t_command				*command_new(t_command *lst);
t_command				*get_command(t_cmds *lst);
t_command				*get_commands(t_cmds *lst);
void					commands_clear(t_command **command_list);
void					ft_clear(t_data *data);
void					print_array(char **str);
void					printlist(void *tmp);
int						ft_fork(void);
void					ft_close(int fd, char *str);
int						executing(t_data *data);
void					only_builtins(t_data *data, t_command *list, int builtin_command);
void					change_directory(t_env *list, char **command_args);
void					ft_echo(char **command_args, bool flag, int i);
void					exiterror(void);
bool					check_n_flag(char *str);
t_env					*env_new(t_env *lst, char *str);
t_env					*env_last(t_env *lst);
char					**linked_list_to_array(t_env *list);
char					**sort_export_array(char **arr, int n);
void					senv_clear(t_env **lst);
char					**linked_list_to_array(t_env *list);
t_env					*unset_env(t_env *list, char **command_args, t_data *data);
void					exit_myminishell(char **command_args, int flag);
char					*get_my_path(t_env *list, char **command_args, bool flag, int i);
void					export(t_env *list, char **command_args, char c, int i);
int						print_export_variables(t_env *list);
int						get_environment_length(char **env);
void					exiterror(void);
t_env					*copy_environment(char **env);
void					ft_putendle(char *str, int fd);
void					print_environment(t_env *list);
t_env					*find_env_variable(t_env *list, char *variable_name);
void					change_directory_path(t_env *list, char *path);
int						morethan2arg(char **command_args);
void					print_working_directory(t_env *env);
int						array_size(char **var);
void					swap(char **s1, char **s2);
int						count_nodes(t_env *list);
int						get_cmd_size(t_cmds *list);
void					get_command_done(t_cmds *list, t_cmds *head,
							char **command, bool flag);
char					**create_environment(void);
int						check_quot(char *str);
int						open_heredoc(t_cmds *cmds, int pid, int status,
							bool flag);
int						ft_strcmp_for_heredoc(char *s1, char *s2);
int						get_command_size(t_command *command);
int						wait_pid(int *pid, int cmd_num);
int						execute_command(t_env *list, t_command *command,
							t_data *data, int index);
int						get_command_in_one_char(char **str);
char					*get_content(char **env, char *variable_name);
bool					check_back_for_heredoc(char *str, int index);
void					slist_clear(t_slist **lst);
char					**get_file_name(char *str);
bool					check_next(char *str);
char					**get_name(char *str);
int						is_numeric(char *str);
int						get_2d_size(char **vars, char **lines);
bool					check_eq(char *str);
void					set_env_if_plus(t_env *env_node, char *export_value);
int						how_many_dollar_in(char *str);
bool					check_next_for_both(char *str);
int						check_double(char *cmd, int i);
void					set_env_after_export(t_env *list, char **export, char c,
							bool export_flag);
void					set_env_after_cd(t_env *list, char *key, char *value);
char					*find_variable_value(t_env *list, t_env *head, char *variable_name,
							bool flag);
char					**array_copy(char **str);
t_cmds					*copy_node(char **cmd, t_token token, bool flag);
t_cmds					*copy_single_node(t_cmds *curr, int *i);
t_cmds					*add_head_to_new_list(t_cmds *head, t_cmds *new_head);
t_cmds					*copy_list(t_cmds *curr, char **command, int i);
t_cmds					*merge_lists(t_cmds *list1, t_cmds *list2);
char					**get_key_and_value(char *str, char **ptr, int i,
							int j);
void					ft_puterror_fd(char *str1, char *str2);
void					print_value(char *str);
char					*increment_s1(char *s1);
bool					is_it_inside(char *str);

#endif