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
