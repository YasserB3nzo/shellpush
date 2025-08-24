# 🚀 **Complete Minishell Execution Flow - Line by Line**

This document explains the complete execution flow of the minishell implementation, starting from the main function and following every line of code.

## 📋 **Program Entry Point: `main` Function**

```c
int main(int ac, char **av, char **env)
```

### **Line 69**: Function Parameters
- `ac`: Argument count (should be 1 for minishell)
- `av`: Argument vector (program name)
- `env`: Environment variables from the system

### **Line 71**: Local Variable Declaration
```c
t_data data;
```
- Declares a `t_data` structure that will hold all shell state:
  - File descriptors for pipes
  - Process IDs for child processes  
  - Environment variables (both array and linked list)
  - Current command line input
  - Various flags and parsed command structures

### **Line 73**: Input Validation & Initialization
```c
if (ac != 1 || data_init(av, &data, env) == 0)
    return (1);
```
- **Checks argument count**: Minishell should be called with no arguments
- **Calls `data_init`**: If initialization fails, exit with code 1
- **Returns 1 on error**: Standard error exit code

---

## 🔧 **Initialization: `data_init` Function**

```c
int data_init(char **av, t_data *data, char **env)
```

### **Line 18**: Suppress Unused Parameter Warning
```c
(void)av;
```
- Tells compiler that `av` parameter is intentionally unused

### **Lines 19-20**: Initialize Global Signal Structure
```c
g_signal.ret = 0;
g_signal.ret_exit = 0;
```
- **`g_signal.ret`**: Current command exit status
- **`g_signal.ret_exit`**: Exit status to return when shell exits

### **Lines 21-22**: Initialize Data Flags
```c
data->path_flag = false;
g_signal.ff = 0;
```
- **`path_flag`**: Tracks if PATH environment variable exists
- **`g_signal.ff`**: Signal handling flag

### **Lines 23-24**: Handle Empty Environment
```c
if (!env[0])
    data->path_flag = true;
```
- If no environment variables exist, set path_flag to true
- This handles cases where shell runs without environment

### **Line 25**: Copy Environment to Linked List
```c
data->list_env = copy_environment(env);
```
- Converts environment array to linked list for easier manipulation
- Each environment variable becomes a node in `t_env` linked list

### **Line 26**: Return Success
```c
return (1);
```

---

## 🔄 **Main Loop: `shell_loop` Function**

```c
static void shell_loop(t_data *data)
```

### **Line 56**: Infinite Loop
```c
while (1)
```
- Continues until user exits with Ctrl+D or `exit` command

### **Line 58**: Set Up Signal Handlers
```c
signal_handler();
```
- Configures how shell responds to signals (Ctrl+C, Ctrl+Z, etc.)
- Sets up handlers for SIGINT, SIGQUIT, SIGTSTP

### **Line 59**: Convert Environment to Array
```c
data->env = linked_list_to_array(data->list_env);
```
- Many functions expect environment as `char **` array
- Converts linked list back to array format for this iteration

### **Line 60**: Display Prompt and Read Input
```c
data->line = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
```
- **`readline()`**: GNU Readline library function
- **`\001\033[1;35m\002`**: ANSI escape codes for purple prompt
- **`minishell$ `**: The actual prompt text
- **`\001\033[0m\002`**: Reset color to normal
- **`\001` and `\002`**: Tell readline these are non-printing characters

### **Lines 61-62**: Check for EOF (Ctrl+D)
```c
if (!data->line)
    break ;
```
- `readline()` returns NULL when user presses Ctrl+D
- This exits the shell gracefully

### **Line 63**: Process the Input Line
```c
process_line(data);
```
- Handles the user's input (explained next)

### **Lines 64-66**: Cleanup for Next Iteration
```c
free(data->line);
free_array(data->env);
g_signal.ff = 0;
```
- Free the input line memory
- Free the environment array (linked list persists)
- Reset signal flag

---

## 📝 **Input Processing: `process_line` Function**

```c
static void process_line(t_data *data)
```

### **Line 36**: Check for Empty/Whitespace-Only Input
```c
if (is_spaces(data->line) == 0)
```
- **`is_spaces()`** returns:
  - **0**: String contains only spaces/tabs or is empty
  - **1**: String contains non-whitespace characters

### **Lines 37-41**: Handle Empty Commands
```c
{
    add_history(data->line);
    g_signal.ret = 0;
    g_signal.ret_exit = 0;
}
```
- **`add_history()`**: Adds to readline history (up/down arrows)
- Set exit status to 0 (success) for empty commands
- This matches bash behavior

### **Line 42**: Check for Valid Quotation
```c
else if (check_quotation(data->line) != -1)
```
- **`check_quotation()`** returns:
  - **0**: All quotes are properly closed
  - **-1**: Unclosed quotes found

### **Lines 43-47**: Process Valid Commands
```c
{
    add_history(data->line);
    parsing(data, NULL, NULL, -1);
    g_signal.ret_exit = g_signal.ret;
}
```
- Add to history
- **`parsing()`**: The main parsing and execution function
- Update final exit status with command result

### **Lines 48-52**: Handle Syntax Errors
```c
else
{
    g_signal.ret = 2;
    g_signal.ret_exit = 2;
}
```
- Set exit status to 2 for syntax errors (like bash)
- This includes unclosed quotes, invalid redirections, etc.

---

## 🧹 **Cleanup: `cleanup_and_exit` Function**

```c
static void cleanup_and_exit(t_data *data)
```

### **Line 30**: Free Environment Array
```c
if (data->env)
    free_array(data->env);
```
- Free the current environment array if it exists

### **Line 31**: Free Environment Linked List
```c
senv_clear(&data->list_env);
```
- Free all nodes in the environment linked list

### **Line 32**: Print Exit Message
```c
ft_putstr_fd("exit\n", 2);
```
- Print "exit" to stderr (file descriptor 2)
- Matches bash behavior when exiting

---

## 🎯 **Program Exit: Back to `main`**

### **Lines 74-76**: Execute Main Logic
```c
shell_loop(&data);
cleanup_and_exit(&data);
return (g_signal.ret_exit);
```
- Run the main shell loop
- Clean up all resources
- **Return final exit status** (this preserves the last command's exit code!)

---

## 🌍 **Global Variables**

### **Line 15**: Global Signal Structure
```c
t_signal g_signal;
```
- **Why global?** Signal handlers can only access global variables
- **`ret`**: Current command exit status
- **`ret_exit`**: Final exit status for shell
- **`sig`**: Signal number received  
- **`ff`**: Flag for signal handling state

---

## 📊 **Key Data Structures**

### **t_data Structure**
```c
struct s_data
{
    int     fd[2];          // Pipe file descriptors
    int     *pid;           // Array of child process IDs
    int     k;              // Process counter
    int     fd_in;          // Input file descriptor
    char    **env;          // Environment array
    char    **cmds;         // Parsed commands
    char    *line;          // Current input line
    bool    path_flag;      // PATH variable exists flag
    bool    flag;           // General purpose flag
    t_env   *list_env;      // Environment linked list
    t_cmds  *lst;           // Command tokens linked list
    t_command *list;        // Final command structures
};
```

### **t_signal Structure**
```c
typedef struct s_signal
{
    int ret;        // Current exit status
    int ret_exit;   // Final exit status
    int sig;        // Signal number
    int ff;         // Signal flag
} t_signal;
```

### **t_env Structure**
```c
typedef struct s_env
{
    char        *var_name;  // Environment variable (NAME=value)
    struct s_env *next;     // Next node
    struct s_env *prev;     // Previous node
} t_env;
```

---

## 🔗 **Key Data Flow**

1. **Program Start**
   - `main()` validates arguments
   - `data_init()` initializes all structures
   - Global signal structure is set up

2. **Main Shell Loop** (`shell_loop`)
   - Set up signal handlers for each iteration
   - Convert environment linked list to array
   - Display colored prompt and read user input
   - Check for EOF (Ctrl+D) to exit
   - Process the input line
   - Clean up memory for next iteration

3. **Input Processing** (`process_line`)
   - **Empty/Whitespace**: Add to history, set exit status 0
   - **Valid Command**: Add to history, parse and execute
   - **Syntax Error**: Set exit status 2

4. **Program Exit**
   - Clean up all allocated memory
   - Print "exit" message
   - Return final exit status

---

## 💡 **Design Patterns Used**

### **State Machine Pattern**
- Different processing paths based on input type
- Clear state transitions between empty/valid/invalid commands

### **Resource Management Pattern**
- Systematic allocation and deallocation
- Environment exists in both array and linked list forms
- Careful cleanup in every loop iteration

### **Signal-Safe Programming**
- Global variables for signal handler communication
- Signal handlers set flags that main loop checks

### **Modular Design**
- Each function has a single, clear responsibility
- Easy to test and debug individual components

---

## 🚨 **Critical Features**

### **Signal Handling**
- **Ctrl+C (SIGINT)**: Sets `g_signal.ret = 130`
- **Ctrl+\\ (SIGQUIT)**: Ignored in interactive mode
- **Ctrl+Z (SIGTSTP)**: Ignored (no job control)

### **Exit Status Management**
- **Empty commands**: Exit status 0
- **Successful commands**: Exit status from command
- **Syntax errors**: Exit status 2
- **Command not found**: Exit status 127
- **Signal interruption**: Exit status 130

### **Memory Safety**
- All `malloc` allocations have corresponding `free`
- Environment array recreated each iteration
- Input line freed after processing
- Comprehensive cleanup on exit

### **Bash Compatibility**
- Prompt format and colors
- History management
- Exit status behavior
- Error message format
- Signal handling behavior

---

## 🧪 **Testing Scenarios**

1. **Empty Input**: Press Enter → Exit status 0
2. **Valid Command**: `echo hello` → Execute and return command status
3. **Invalid Syntax**: `echo "unclosed` → Exit status 2
4. **Signal Handling**: Ctrl+C → Exit status 130
5. **EOF**: Ctrl+D → Exit with last command status

---

## 🔧 **Compilation & Usage**

```bash
# Compile
make

# Run
./minishell

# Test exit status
echo $?
```

This architecture creates a robust, bash-compatible shell that properly handles user input, executes commands, manages environment variables, and maintains correct exit status behavior throughout its execution!

---

## ⚙️ **EXECUTION PART - THE CORE ENGINE**

The execution part is where the magic happens! After parsing user input, the shell needs to execute commands, handle pipes, manage file descriptions, and run built-in commands. Let's dive deep into this critical component.

---

## 🎯 **Parsing Function: Entry Point to Execution**

```c
void parsing(t_data *data, t_cmds *cmd_head, char *str, int i)
```

### **Parameter Analysis**
- `data`: Main shell data structure
- `cmd_head`: Head of command tokens (initially NULL)
- `str`: String buffer for building commands (initially NULL)  
- `i`: Index counter (initially -1)

### **Core Parsing Flow**
1. **Tokenization**: Breaks input into tokens (words, operators, quotes)
2. **Quote Processing**: Handles single and double quotes with variable expansion
3. **Command Building**: Groups tokens into executable commands
4. **Redirection Parsing**: Identifies `<`, `>`, `>>`, `<<` operators
5. **Pipe Detection**: Finds `|` operators for command chaining

### **Execution Trigger**
After parsing completes, it calls:
```c
executing(data);
```
This is where the real execution begins!

---

## 🚀 **Main Execution Engine: `executing` Function**

```c
void executing(t_data *data)
```

### **Environment Setup**
```c
data->env = linked_list_to_array(data->list_env);
```
- Converts environment linked list to array format
- Many system calls expect `char **env` format

### **Command Classification**
The function analyzes the parsed commands and determines execution strategy:

1. **Single Built-in Command**: Execute directly in current shell process
2. **Single External Command**: Fork and execute
3. **Pipeline Commands**: Set up pipes and execute chain
4. **Commands with Redirections**: Handle file I/O redirection

---

## 🔄 **Pipeline Execution: `with_pipe` Function**

```c
void with_pipe(t_data *data, t_command *list)
```

This is the **heart of command execution**. It handles:
- Single commands
- Multi-command pipelines
- Built-in vs external command routing

### **Memory Allocation for Process Tracking**
```c
data->pid = (int *)malloc(sizeof(int) * how_many_cmds(list));
```
- Allocates array to store child process IDs
- **Why?** Need to wait for all children to complete
- **Size**: Number of commands in the pipeline

### **Single Command Execution**
```c
if (how_many_cmds(list) == 1)
{
    // Handle single command (no pipes needed)
    execute_command(list, data);
}
```

### **Pipeline Execution Loop**
For multiple commands connected by pipes:
```c
while (list)
{
    pipe(data->fd);  // Create pipe for this command
    // Fork child process
    // Set up input/output redirection
    // Execute command
    list = list->next;
}
```

---

## 🔧 **Individual Command Execution: `execute_command` Function**

```c
void execute_command(t_command *command, t_data *data)
```

### **Built-in Detection**
```c
int builtin_id = get_command_in_one_char(command->cmd);
if (builtin_id != 0)
{
    run_builtins(builtin_id, command, data, 0);
    return;
}
```

### **Built-in Command IDs**
- **1**: `cd` (change directory)
- **2**: `pwd` (print working directory)
- **3**: `env` (print environment)
- **4**: `export` (set environment variables)
- **5**: `unset` (remove environment variables)
- **6**: `exit` (exit shell)
- **7**: `echo` (print arguments)

### **External Command Path Resolution**
```c
char *path = get_path_command(command->cmd[0], data->env);
```
- Searches PATH environment variable
- Returns full path to executable
- Returns NULL if command not found

### **Process Forking for External Commands**
```c
pid_t pid = fork();
if (pid == 0)
{
    // Child process
    execute_command_part_three(command->cmd, command, data, path);
}
else
{
    // Parent process
    data->pid[data->k++] = pid;  // Store child PID
    wait(&status);               // Wait for completion
}
```

---

## 🏗️ **Built-in Command Execution: `run_builtins` Function**

```c
int run_builtins(int cmd_id, t_command *command, t_data *data, int flag)
```

### **Command Router**
```c
if (cmd_id == 1)
    change_directory(env_list, command->cmd);           // cd
else if (cmd_id == 2) 
    print_working_directory(env_list);                  // pwd
else if (cmd_id == 3)
    print_environment(env_list);                        // env
else if (cmd_id == 4)
    export(env_list, command->cmd, '-', 1);             // export
else if (cmd_id == 5)
    handle_unset_command(data, command);                // unset
else if (cmd_id == 6)
    exit_myminishell(command->cmd, flag);               // exit
else if (cmd_id == 7)
    execute_echo_command(command->cmd + 1, true, 0);    // echo
```

### **Why Built-ins are Special**
- **Environment Changes**: Only the shell process can modify its own environment
- **Working Directory**: `cd` must change the shell's working directory
- **Exit**: Only the shell can exit itself
- **Efficiency**: No need to fork for simple operations

---

## 📁 **File Redirection Handling**

### **Input Redirection (`<`)**
```c
if (command->input_file)
{
    int fd_in = open(command->input_file, O_RDONLY);
    dup2(fd_in, STDIN_FILENO);  // Redirect stdin
    close(fd_in);
}
```

### **Output Redirection (`>` and `>>`)**
```c
if (command->output_file)
{
    int flags = (command->append) ? (O_WRONLY | O_CREAT | O_APPEND) 
                                  : (O_WRONLY | O_CREAT | O_TRUNC);
    int fd_out = open(command->output_file, flags, 0644);
    dup2(fd_out, STDOUT_FILENO);  // Redirect stdout
    close(fd_out);
}
```

### **Heredoc (`<<`)**
```c
if (command->heredoc_delimiter)
{
    int pipefd[2];
    pipe(pipefd);
    // Write heredoc content to pipe
    // Redirect pipe read end to stdin
}
```

---

## 🔗 **Pipeline Mechanics Deep Dive**

### **Pipe Creation and Management**
```c
int pipe_fd[2];
pipe(pipe_fd);  // Creates [read_end, write_end]
```

### **Command Chain Setup**
For command sequence: `cmd1 | cmd2 | cmd3`

```c
// cmd1: output goes to pipe1
dup2(pipe1[1], STDOUT_FILENO);  // Redirect stdout to pipe write
close(pipe1[0]);                // Close unused read end

// cmd2: input from pipe1, output to pipe2  
dup2(pipe1[0], STDIN_FILENO);   // Redirect stdin from pipe read
dup2(pipe2[1], STDOUT_FILENO);  // Redirect stdout to next pipe
close(pipe1[1]);                // Close unused write end
close(pipe2[0]);                // Close unused read end

// cmd3: input from pipe2
dup2(pipe2[0], STDIN_FILENO);   // Redirect stdin from pipe read
close(pipe2[1]);                // Close unused write end
```

---

## 🎭 **Process Management and Synchronization**

### **Child Process Tracking**
```c
data->pid = malloc(sizeof(int) * command_count);
data->k = 0;  // Process counter

// For each command in pipeline:
pid_t pid = fork();
if (pid == 0)
{
    // Child process - execute command
}
else
{
    data->pid[data->k++] = pid;  // Store PID in parent
}
```

### **Waiting for Pipeline Completion**
```c
for (int i = 0; i < data->k; i++)
{
    int status;
    waitpid(data->pid[i], &status, 0);
    
    // Extract exit status from last command
    if (i == data->k - 1)  // Last command in pipeline
    {
        if (WIFEXITED(status))
            g_signal.ret = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_signal.ret = 128 + WTERMSIG(status);
    }
}
```

---

## 🚨 **Signal Handling During Execution**

### **Signal Handler Setup**
```c
void signal_handler(void)
{
    signal(SIGINT, signal_handler_heredoc);    // Ctrl+C
    signal(SIGQUIT, SIG_IGN);                  // Ctrl+\ ignored
}
```

### **Signal Handler Implementation**
```c
void signal_handler_heredoc(int sig)
{
    if (sig == SIGINT)  // Ctrl+C pressed
    {
        g_signal.ret = 130;  // Standard Ctrl+C exit code
        // In child processes, this will terminate execution
        // In parent, it sets exit status and continues
    }
}
```

### **Signal Propagation in Pipelines**
- **Parent Process**: Receives signal, sets exit status
- **Child Processes**: Inherit signal handling, may be terminated
- **Built-ins**: Execute in parent, affected by signals immediately

---

## 🔍 **Path Resolution Algorithm**

### **`get_path_command` Function**
```c
char *get_path_command(char *command, char **env)
```

### **Algorithm Steps**
1. **Absolute/Relative Path Check**:
   ```c
   if (command[0] == '/' || command[0] == '.')
       return (ft_strdup(command));  // Use as-is
   ```

2. **PATH Environment Variable Lookup**:
   ```c
   char *path_env = get_env_value("PATH", env);
   char **paths = ft_split(path_env, ':');
   ```

3. **Directory Search**:
   ```c
   for (int i = 0; paths[i]; i++)
   {
       char *full_path = ft_strjoin(paths[i], "/", command);
       if (access(full_path, X_OK) == 0)  // Executable exists
           return (full_path);
   }
   ```

4. **Command Not Found**:
   ```c
   return (NULL);  // Command not found in PATH
   ```

---

## 🏃‍♂️ **External Command Execution: `execute_command_part_three`**

```c
void execute_command_part_three(char **com, t_command *command, 
                               t_data *data, char *path)
```

### **Final Execution Steps**
1. **Built-in Check** (last chance):
   ```c
   int cmd = get_command_in_one_char(com);
   if (cmd != 0)
   {
       run_builtins(cmd, command, data, 1);
       return;
   }
   ```

2. **Signal Handler Setup for Child**:
   ```c
   signal(SIGQUIT, printsignalsc);  // Enable Ctrl+\ for child processes
   ```

3. **Execute External Command**:
   ```c
   execve(path, com, data->env);
   ```

4. **Error Handling** (if execve fails):
   ```c
   ft_putstr_fd("minishell: ", 2);
   ft_putstr_fd(com[0], 2);
   ft_putstr_fd(": ", 2);
   
   if (stat(path, &stats) == 0 && S_ISDIR(stats.st_mode))
       ft_putendl_fd("Is a directory", 2);
   else
       ft_putendl_fd(strerror(errno), 2);
   
   exit(-1);  // Child process exits with error
   ```

---

## 🎯 **Built-in Commands Deep Dive**

### **`cd` Command Implementation**
```c
void change_directory(t_env *env_list, char **cmd)
{
    char *target_dir;
    
    if (!cmd[1])  // No argument - go to HOME
        target_dir = get_env_value("HOME", env_list);
    else
        target_dir = cmd[1];
    
    if (chdir(target_dir) == -1)
    {
        perror("cd");
        g_signal.ret = 1;
    }
    else
    {
        // Update PWD and OLDPWD environment variables
        update_pwd_env(env_list);
        g_signal.ret = 0;
    }
}
```

### **`export` Command Implementation**
```c
void export(t_env *env_list, char **cmd, char c, int flag)
{
    if (!cmd[1])  // No arguments - print all exports
    {
        print_sorted_env(env_list);
        return;
    }
    
    for (int i = 1; cmd[i]; i++)
    {
        if (validate_identifier(cmd[i]))
            add_or_update_env(&env_list, cmd[i]);
        else
        {
            ft_putendl_fd("export: not a valid identifier", 2);
            g_signal.ret = 1;
        }
    }
}
```

### **`unset` Command Implementation**
```c
t_env *unset_env(t_env *env_list, char **cmd, t_data *data)
{
    for (int i = 1; cmd[i]; i++)
    {
        env_list = remove_env_var(env_list, cmd[i]);
    }
    return (env_list);
}
```

---

## 📊 **Execution Flow Summary**

### **Single Command Flow**
```
Input → Parse → Built-in Check → Execute → Set Exit Status
```

### **Pipeline Flow**  
```
Input → Parse → Create Pipes → Fork Processes → Execute → Wait All → Set Exit Status
```

### **Command with Redirection Flow**
```
Input → Parse → Open Files → Set up FD Redirection → Execute → Close FDs → Set Exit Status
```

---

## 💾 **Memory Management During Execution**

### **Dynamic Allocations**
1. **Process ID Array**: `malloc(sizeof(int) * command_count)`
2. **Environment Array**: Recreated each iteration
3. **Path Strings**: Allocated during path resolution
4. **Command Structures**: Built during parsing

### **Cleanup Strategy**
1. **After Each Command**: Free temporary allocations
2. **After Pipeline**: Free PID array, close all file descriptors
3. **Before Exit**: Free all persistent data structures

---

## 🔧 **Error Handling Patterns**

### **Command Not Found (127)**
```c
if (!path)
{
    ft_putendl_fd("command not found", 2);
    g_signal.ret = 127;
}
```

### **Permission Denied (126)**
```c
if (access(path, X_OK) != 0)
{
    ft_putendl_fd("Permission denied", 2);
    g_signal.ret = 126;
}
```

### **System Call Failures**
- **fork() fails**: Print error, continue with next command
- **pipe() fails**: Print error, exit pipeline
- **execve() fails**: Print specific error, exit child process

---

## 🏆 **Execution Engine Features**

### **✅ Implemented Features**
- **Pipeline Execution**: Multiple commands connected by pipes
- **Built-in Commands**: 7 essential built-ins implemented
- **File Redirection**: Input (`<`), output (`>`), append (`>>`), heredoc (`<<`)
- **Path Resolution**: Searches PATH environment variable
- **Signal Handling**: Proper Ctrl+C, Ctrl+\ behavior
- **Exit Status Management**: Correct exit codes for all scenarios
- **Process Management**: Fork, exec, wait cycle with proper synchronization
- **Environment Manipulation**: Export, unset, and environment inheritance

### **🎯 Bash Compatibility**
- **Command Resolution**: Same search order as bash
- **Exit Codes**: Identical to bash behavior
- **Signal Behavior**: Matches bash signal handling
- **Built-in Semantics**: cd, export, etc. work like bash
- **Pipeline Status**: Returns exit status of last command in pipeline

---

This execution engine provides a complete, robust command execution system that handles the complexity of process management, I/O redirection, and built-in command integration while maintaining full compatibility with bash behavior!
