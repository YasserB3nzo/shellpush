# Shell Execution Flow Documentation

This document explains the execution flow of the shell implementation, detailing each function and its role in the process.

## Main Execution Flow

### 1. `executing(t_data *data)`
The main execution function that orchestrates the entire command execution process.

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
Manages command execution with pipes:
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
