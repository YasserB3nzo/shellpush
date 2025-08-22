# Shell Execution Flow Documentation

This document explains the execution flow of the shell implementation, detailing each function and its role in the process.

## Main Execution Flow

### 1. `executing(t_data *data)`
The main execution function that orchestrates the entire command execution process.

**Code:**
```c
int executing(t_data *data)
{
    t_command   *command_list;
    int         builtin_command;
    bool        flag;

    command_list = data->list;
    flag = false;
    builtin_command = get_command_in_one_char(command_list->cmd);
    if (!command_list || (command_list->cmd && command_list->cmd[0][0] == '\n'))
        return (2);
    if (!command_list->next)
        flag = change_underscore(data, command_list, NULL, 0);
    if (builtin_command != 0 && !command_list->next)
        only_builtins(data, command_list, builtin_command);
    else
        with_pipe(data, command_list);
    if (flag == true)
        change_underscore(data, command_list, NULL, 0);
    g_signal.ret_exit = g_signal.ret;
    return (g_signal.ret);
}
```

**Flow:**
1. Gets the command list and checks for built-in commands
2. Validates empty commands and newlines
3. Updates the underscore environment variable
4. Decides between:
   - Single command execution (built-ins)
   - Pipeline execution
5. Returns the execution status

### 2. Command Type Detection

#### `get_command_in_one_char(char **str)`
- Identifies built-in commands
- Returns command IDs:
  - 1: cd
  - 2: pwd
  - 3: env
  - 4: export
  - 5: unset
  - 6: exit
  - 7: echo
  - 0: external command

### 3. Built-in Command Execution

#### `only_builtins(t_data *data, t_command *list, int builtin_command)`
Handles execution of built-in commands.

**Annotated Code:**
```c
void only_builtins(t_data *data, t_command *list, int builtin_command)
{
    int in;                  // backup of STDIN
    int out;                 // backup of STDOUT

    in = dup(STDIN_FILENO);   // duplicate current STDIN
    out = dup(STDOUT_FILENO); // duplicate current STDOUT
    if (list->infile || list->outfile)
        g_signal.ret = hand_the_redirectionin(list); // apply redirections first
    if (g_signal.ret != 1)    // only run builtin if redirections succeeded
        run_builtins(builtin_command, list, data, 0);
    // Restore original stdin if it was redirected; else close the backup
    if ((g_signal.ret != 1) && list->infile)
        dup2(in, STDIN_FILENO);
    else
        ft_close(in, "in");
    // Restore original stdout if it was redirected; else close the backup
    if ((g_signal.ret != 1) && list->outfile)
        dup2(out, STDOUT_FILENO);
    else
        ft_close(out, "out");
}
```

**Process:**
1. Saves original stdin/stdout
2. Applies redirections (if any)
3. Executes the builtin
4. Restores standard descriptors

### 4. Pipeline Execution

#### `with_pipe(t_data *data, t_command *list)`
Manages command execution with pipes.

**Code:**
```c
void with_pipe(t_data *data, t_command *list)
{
    data->k = 0;
    data->pid = malloc(sizeof(int) * (get_command_size(list)));
    data->fd_in = STDIN_FILENO;
    while (list)
    {
        if (list->next)
        {
            if (pipe(data->fd) == -1)
                break;
        }
        g_signal.ret = execute_command(data->list_env, list, data, data->k++);
        if (list->next)
            ft_close(data->fd[1], "data->fd[0]");
        if (list->prev)
            ft_close(data->fd_in, "data->fd[1]");
        if (list->next)
            data->fd_in = data->fd[0];
        list = list->next;
    }
    if (g_signal.ret == 0 && data->k != 0)
        g_signal.ret = wait_pid(data->pid, data->k);
    free(data->pid);
    data->pid = NULL;
}
```

**Process:**
1. Allocates process ID array
2. For each command:
   - Creates pipes if needed
   - Executes command
   - Manages file descriptors
   - Tracks child processes
3. Waits for all processes to complete
4. Cleans up resources

### 5. Command Execution

#### `execute_command(t_env *list, t_command *command, t_data *data, int index)`
Handles individual command execution.

**Helper Parts (Annotated):**
```c
// Decide redirections & basic validation before executing a command path
void execute_command_part_one(char **com, t_command *command, t_data *data, char *path)
{
    int red = 0;
    signal(SIGINT, SIG_DFL);          // child restores default SIGINT behavior
    free(data->line);                  // free the original input line (no longer needed)
    if (command->infile || command->outfile)
        red = hand_the_redirectionin(command); // apply IO redirections in child
    if (red == 1)                      // failure opening a file -> exit with error
    {
        senv_clear(&data->list_env);
        free_array(data->env);
        exit(1);
    }
    else if (path == NULL && get_command_in_one_char(com) == 0)
    {   // Not a builtin and no path -> command not found
        ft_putstr_fd("minishell: ", 2);
        if (((com[0][0] == 39 && com[0][1] == 39) || (com[0][0] == 34 && com[0][1] == 34)) && com[0][2] == '\0')
            ft_putstr_fd("'': command not found\n", 2); // empty quotes case
        else
            ft_puterror_fd(com[0], ": command not found\n");
        senv_clear(&data->list_env);
        free_array(data->env);
        exit(127); // standard code for command-not-found
    }
}

// Setup pipe ends (stdin/stdout) for the current command in the pipeline
void execute_command_part_two(t_command *command, t_data *data)
{
    if (command->prev && !command->infile)          // if there is a previous command and no explicit infile, read from previous pipe
    {
        dup2(data->fd_in, STDIN_FILENO);
        ft_close(data->fd_in, "fd_in\n");
    }
    if (command->next && !command->outfile)         // if there is a next command and no explicit outfile, pipe stdout forward
        dup2(data->fd[1], STDOUT_FILENO);
    if (command->next)                              // close both ends in child after duplicating
    {
        ft_close(data->fd[1], "fd[0]\n");
        ft_close(data->fd[0], "fd[1]\n");
    }
}

// Execute builtin or external command
void execute_command_part_three(char **com, t_command *command, t_data *data, char *path)
{
    struct stat stats;
    int cmd = get_command_in_one_char(com);
    if (cmd == 0) // external command
    {
        if (signal(SIGQUIT, printsignalsc) != SIG_ERR)
            g_signal.ret = 131; // set if SIGQUIT handler installed
        execve(path, com, data->env);  // on success no return
        // If execve returns, there was an error
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(com[0], 2);
        ft_putstr_fd(": ", 2);
        stat(path, &stats);
        if (S_ISDIR(stats.st_mode))
            ft_putendle("Is a directory", 2);
        else
            ft_putendle(strerror(errno), 2);
        senv_clear(&data->list_env);
        free_array(data->env);
        exit(-1);
    }
    else
        run_builtins(cmd, command, data, 1); // run builtin in child (flag=1)
}
```

**Main Wrapper (Annotated):**
```c
int execute_command(t_env *list, t_command *command, t_data *data, int index)
{
    char *path;

    if (!command->cmd)                      // empty node
        return (-1);
    if (command->cmd[0][0] == '\0' && !command->infile && !command->outfile)
        return (0);                         // blank command with no redirs
    path = get_my_path(list, command->cmd, data->path_flag, 0); // resolve path if external
    signal(SIGINT, SIG_IGN);                // parent ignores SIGINT while forking
    data->pid[index] = ft_fork();           // create child
    if (data->pid[index] == 0)              // CHILD PROCESS
    {
        execute_command_part_one(command->cmd, command, data, path);
        execute_command_part_two(command, data);
        if (command->cmd[0][0] != '\0')
            execute_command_part_three(command->cmd, command, data, path);
        senv_clear(&data->list_env);        // safety cleanup if nothing executed
        free_array(data->env);
        exit(0);
    }
    free(path);                             // PARENT cleans path copy
    if (data->pid[index] < 0)               // fork error
        return (-1);
    return (0);
}
```

**Summary:** Splits execution into validation/redirection, pipe setup, then builtin/external dispatch for clarity and error handling.

### 6. Redirection Handling

#### `hand_the_redirectionin(t_command *lst)`
Manages input/output redirections.

**Supporting Functions (Annotated):**
```c
// Count nodes in a redirection list
int get_files_num(t_slist *list)
{
    int size = 0;
    while (list) { size++; list = list->next; }
    return size;
}

// Close an array of fds terminated by sentinel -11
void ft_ft_close(int *fd)
{
    int i = 0;
    while (fd && fd[i] != -11)
    {
        if (fd[i] == -1)
            break;          // error marker
        ft_close(fd[i], "fd[i]");
        i++;
    }
    if (fd) free(fd);
}

// Print open() failure nicely and cleanup
int *print_open_error(int *fd, char *str)
{
    ft_ft_close(fd);
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(str, 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd(strerror(errno), 2);
    ft_putchar_fd('\n', 2);
    return NULL;
}

// Open every file in list in sequence; dup2 the latest appropriate fd
int *ft_open(t_slist *list, int size, int j)
{
    int *fd;

    size = get_files_num(list);
    fd = malloc((size + 1) * sizeof(int));
    fd[size] = -11; // sentinel
    while (list)
    {
        if (list->token == Infile)
            fd[j] = open(list->cmd, O_RDONLY);
        else if (list->token == OutFile)
            fd[j] = open(list->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        else if (list->token == AppendFile)
            fd[j] = open(list->cmd, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd[j] == -1)
            return print_open_error(fd, list->cmd);
        if (list->token == Infile)
            dup2(fd[j], STDIN_FILENO);          // latest infile becomes stdin
        else if (list->token == OutFile || list->token == AppendFile)
            dup2(fd[j], STDOUT_FILENO);         // latest outfile/append becomes stdout
        j++;
        list = list->next;
    }
    return fd; // caller closes via ft_ft_close
}
```

**Main Redirection Function:**
```c
int hand_the_redirectionin(t_command *lst)
{
    int *filein = NULL;
    int *fileout = NULL;

    if (lst->infile != NULL)
    {
        filein = ft_open(lst->infile, 0, 0);
        if (!filein)
            return 1; // failure opening some infile
    }
    if (lst->outfile)
    {
        fileout = ft_open(lst->outfile, 0, 0);
        if (!fileout)
            return 1; // failure opening some outfile
    }
    if (filein)
        ft_ft_close(filein);  // close all kept fds (dup2 already applied)
    if (fileout)
        ft_ft_close(fileout);
    return 0;
}
```

**Notes:** Multiple redirections override earlier ones (last wins) matching POSIX shell behavior.

## Signal Handling

### 1. `signal_handler(void)`
Sets up main signal handlers:
- SIGQUIT (Ctrl+\\): Ignored
- SIGINT (Ctrl+C): Custom handler
- SIGTSTP (Ctrl+Z): Ignored

**Code (Annotated):**
```c
void signal_handler(void)
{
    signal(SIGQUIT, SIG_IGN);      // ignore Ctrl+\ so shell isn't killed
    signal(SIGINT, ft_handle_sigint); // custom Ctrl+C handling (interactive)
    signal(SIGTSTP, SIG_IGN);      // ignore Ctrl+Z (prevent suspension)
}
```

### 2. `ft_handle_sigint(int sig)`
Handles interrupt signal (Ctrl+C):
1. Checks signal blocking flag
2. Prints newline
3. Clears current line
4. Redraws prompt
5. Sets return code to 130

**Code (Annotated):**
```c
void ft_handle_sigint(int sig)
{
    if (g_signal.ff == 1) // flag: ignore during critical sections
        return;
    (void)sig;
    printf("\n");           // move to fresh line
    rl_replace_line("", 1);  // clear current input buffer
    rl_on_new_line();         // readline internal state update
    rl_redisplay();           // redraw prompt
    g_signal.ret = 130;       // set exit status to SIGINT code
}
```

### 3. `signal_hand2(int s)`
Secondary signal handler:
- Restores default SIGINT handling
- Prints newline

**Code (Annotated):**
```c
void signal_hand2(int s)
{
    (void)s;
    signal(SIGINT, SIG_DFL); // restore default (used in heredoc or subshell contexts)
    write(1, "\n", 1);
}
```

## Environment Variable Management

### 1. `change_underscore(t_data *data, t_command *command, char *str, int i)`
Updates the special "_" variable (stores last argument of last executed command).

**Annotated Code:**
```c
bool change_underscore(t_data *data, t_command *command, char *str, int i)
{
    t_env *env_node;

    if (!command->cmd || !command->cmd[0])
        return false;                 // nothing to update
    while (command->cmd[i])           // find last argument
        i++;
    str = command->cmd[--i];
    if (!str || *str == '\n')
        return false;                 // ignore pure newline
    env_node = find_env_variable(data->list_env, "_");
    if (env_node)
    {   // replace existing _= value
        free(env_node->var_name);
        env_node->var_name = ft_strjoin("_=", str);
    }
    else
    {   // create new node at end of env list
        env_node = env_new(data->list_env, ft_strjoin("_=", str));
        data->list_env = env_last(data->list_env);
        if (data->list_env)
            data->list_env->next = env_node;
        else
            data->list_env = env_node;
    }
    return true;
}
```

## Process Management

### 1. `wait_pid(int *pid, int cmd_num)`
Handles process completion.

**Annotated Code:**
```c
int wait_pid(int *pid, int cmd_num)
{
    int i;
    int status;

    if (cmd_num == 0)
        return 0;                   // nothing to wait for
    i = cmd_num;
    status = 0;
    waitpid(pid[--i], &status, 0);  // wait last command first (determine pipeline status)
    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGQUIT)
            write(2, "Quit (core dumped)\n", 20);
        if (WTERMSIG(status) == SIGINT)
            write(2, "\n", 1);
    }
    if (WIFEXITED(status))
        status = WEXITSTATUS(status); // extract exit code
    while (i >= 0)                    // reap remaining children (ignore their statuses)
        waitpid(pid[i--], 0, 0);
    if (g_signal.ret == 130)          // override with SIGINT code if set globally
        return 130;
    return status;                    // return last command status (shell semantics)
}
```

## Error Handling

Error handling is integrated throughout the execution process:
- Command validation
- File descriptor management
- Process creation
- Signal handling
- Exit status propagation

## Memory Management

The execution system carefully manages resources:
- File descriptors are properly closed
- Process IDs are tracked and freed
- Environment variables are properly allocated/deallocated
- Pipe descriptors are managed and cleaned up

## Return Values

The execution system uses several return values:
- 0: Successful execution
- 1: General error
- 2: Command not found
- 126: Command found but not executable
- 127: Command not found
- 130: Terminated by Ctrl+C
- Other values: Command-specific exit codes
