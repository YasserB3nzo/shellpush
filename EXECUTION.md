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
Handles execution of built-in commands:
1. Saves original stdin/stdout
2. Handles redirections if present
3. Executes the built-in command
4. Restores original stdin/stdout

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
Handles individual command execution:
1. Forks a new process
2. In child process:
   - Sets up redirections
   - Executes command (built-in or external)
3. In parent process:
   - Manages pipes
   - Tracks process ID

### 6. Redirection Handling

#### `hand_the_redirectionin(t_command *lst)`
Manages input/output redirections:
1. Processes input files
2. Processes output files
3. Sets up file descriptors
4. Handles heredocs

## Signal Handling

### 1. `signal_handler(void)`
Sets up main signal handlers:
- SIGQUIT (Ctrl+\\): Ignored
- SIGINT (Ctrl+C): Custom handler
- SIGTSTP (Ctrl+Z): Ignored

### 2. `ft_handle_sigint(int sig)`
Handles interrupt signal (Ctrl+C):
1. Checks signal blocking flag
2. Prints newline
3. Clears current line
4. Redraws prompt
5. Sets return code to 130

### 3. `signal_hand2(int s)`
Secondary signal handler:
- Restores default SIGINT handling
- Prints newline

## Environment Variable Management

### 1. `change_underscore(t_data *data, t_command *command, char *str, int i)`
Updates the special "_" variable:
1. Finds last argument of command
2. Updates or creates "_" environment variable
3. Maintains environment list structure

## Process Management

### 1. `wait_pid(int *pid, int cmd_num)`
Handles process completion:
1. Waits for all child processes
2. Collects exit status
3. Handles signals
4. Returns final status

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
