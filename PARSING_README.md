# Minishell Parsing System - Complete Guide

## Table of Contents
1. [Overview](#overview)
2. [Data Structures](#data-structures)
3. [Parsing Flow](#parsing-flow)
4. [Token System](#token-system)
5. [Error Handling](#error-handling)
6. [Variable Expansion](#variable-expansion)
7. [Heredoc Processing](#heredoc-processing)
8. [Node Creation and Piping](#node-creation-and-piping)
9. [Detailed Example](#detailed-example)
10. [Function Reference](#function-reference)

## Overview

The minishell parsing system transforms user input into an executable command structure through multiple phases:
1. **Lexical Analysis**: Breaking input into tokens
2. **Syntax Analysis**: Validating token sequences 
3. **Variable Expansion**: Replacing variables with values
4. **Node Creation**: Building command nodes
5. **Pipeline Processing**: Handling pipes and redirections

## Data Structures

### Core Structures

```c
typedef enum s_token {
    Cmd,            // Command token
    AppendFile,     // Output file for append (>>)
    HereDocDel,     // Heredoc delimiter
    Infile,         // Input file (<)
    OutFile,        // Output file (>)
    Operation,      // Redirection operations
    NonOperation,   // Non-operation tokens
    Input,          // Input redirection (<)
    Output,         // Output redirection (>)
    Append,         // Append redirection (>>)
    HereDoc,        // Heredoc operator (<<)
    Pipe,           // Pipe operator (|)
    Non             // No specific token type
} t_token;

typedef struct s_cmds {
    char **cmd;         // Array of command parts
    t_token token;      // Token type
    t_token operation;  // Operation type
    t_data *data;       // Reference to main data
    struct s_cmds *next;
    struct s_cmds *prev;
} t_cmds;

typedef struct s_command {
    char **cmd;              // Final command array
    t_slist *infile;         // Input file list
    t_slist *outfile;        // Output file list
    struct s_command *next;
    struct s_command *prev;
} t_command;
```

## Parsing Flow

### Phase 1: Initial Validation and Expansion

```c
void parsing(t_data *data, t_cmds *lst, t_command *commands, int i)
{
    // Step 1: Check for invalid operator sequences (<<<, <<<<, etc.)
    g_signal.ret = check_invalid_lt_sequences(data->line);
    if (g_signal.ret != 0) {
        data->cmds = NULL;
        data->lst = NULL;
        return;
    }
    
    // Step 2: Prepare line and tokenize
    if (prepare_line_and_tokens(data, &lst, i) != 0) {
        remove_quotes(lst);
        ft_clear(data);
        return;
    }
    
    // Step 3: Remove quotes and finalize
    remove_quotes(lst);
    if (g_signal.ret == 0 && g_signal.sig != -1)
        finalize_parsing(data, lst, commands);
    else
        ft_clear(data);
}
```

### Phase 2: Line Processing and Token Assignment

```c
static int prepare_line_and_tokens(t_data *data, t_cmds **lst, int i)
{
    // Expand variables in the input line
    data->cmds = get_cmds_done(data, data->cmds);
    
    // Remove extra spaces from each command part
    while (data->cmds[++i]) {
        if (cmdcheck(data->cmds[i]) == 0)
            data->cmds[i] = rm_spaces(data->cmds[i]);
    }
    
    // Create linked list from command array
    get_list(data->cmds, i, lst, data);
    
    // Initialize tokens for each node
    g_signal.ret = init_tokens(*lst, 0, *lst);
    data->lst = *lst;
    
    // Check for syntax errors
    if (g_signal.ret == 0)
        g_signal.ret = errors_managment(data, 0);
        
    return (g_signal.ret);
}
```

### Phase 3: Command Expansion

```c
char **get_cmds_done(t_data *data, char **cmds)
{
    // Expand environment variables and special characters
    data->line = check_expand(data->line, data);
    
    // Split the expanded line into command tokens
    cmds = ft_split_msh(data->line);
    return (cmds);
}
```

The `ft_split_msh` function is a specialized split that handles:
- Quoted strings (preserves spaces within quotes)
- Special characters (<, >, |, <<, >>)
- Escaped characters
- Variable expansion contexts

## Token System

### Token Assignment Process

```c
int init_tokens(t_cmds *cmds, int token_size, t_cmds *lst)
{
    int status;
    
    while (cmds) {
        token_size = ft_strlen(cmds->cmd[0]);
        status = 0;
        
        // Check for sequences of < characters (error case)
        if (token_size >= 3 && cmds->cmd[0][0] == '<')
            status = check_all_lt(cmds, token_size);
        else
            assign_simple_tokens(cmds, token_size);
            
        assign_cmd_token(cmds);
        
        if (status == 2)
            return (2);
        cmds = cmds->next;
    }
    
    non_token(lst);  // Assign default tokens to unassigned nodes
    return (0);
}
```

### Token Assignment Rules

```c
void assign_simple_tokens(t_cmds *cmds, int token_size)
{
    if (token_size == 1 && cmds->cmd[0][0] == '<')
        token1(cmds, '<');          // Input redirection
    else if (token_size == 1 && cmds->cmd[0][0] == '>')
        token1(cmds, '>');          // Output redirection
    else if (token_size == 1 && cmds->cmd[0][0] == '|')
        token1(cmds, '|');          // Pipe
    else if (token_size == 2 && cmds->cmd[0][0] == '>' && cmds->cmd[0][1] == '>')
        token2(cmds, 1);            // Append redirection
    else if (token_size == 2 && cmds->cmd[0][0] == '<' && cmds->cmd[0][1] == '<')
        token2(cmds, 2);            // Heredoc
}
```

### Context-Aware Token Assignment

```c
void token1(t_cmds *cmds, char c)
{
    if (c == '<') {
        if (cmds->prev && cmds->prev->token == Non)
            cmds->prev->token = Cmd;        // Previous becomes command
        if (cmds->next && cmds->next->token == Non)
            cmds->next->token = Infile;     // Next becomes input file
        cmds->token = Input;
    }
    else if (c == '>') {
        if (cmds->prev && cmds->prev->token == Non)
            cmds->prev->token = Cmd;        // Previous becomes command
        if (cmds->next && cmds->next->token == Non)
            cmds->next->token = OutFile;    // Next becomes output file
        cmds->token = Output;
    }
    else if (c == '|') {
        if (cmds->prev && cmds->prev->token == Non)
            cmds->prev->token = Cmd;        // Previous becomes command
        if (cmds->next && cmds->next->token == Non)
            cmds->next->token = Cmd;        // Next becomes command
        cmds->token = Pipe;
    }
}
```

## Error Handling

### Syntax Error Detection

The error management system checks for various syntax violations:

```c
int errors_managment(t_data *data, int i)
{
    t_cmds *curr = data->lst;
    t_cmds *head = curr;
    
    // Process heredocs first (they can modify the structure)
    i = heredoc(head, head, i, 0);
    if (i != 0)
        return (i);
        
    // Check for syntax errors in token sequences
    return (process_tokens_loop(curr, i));
}
```

### Common Error Cases

1. **Pipe Errors**:
   ```c
   int check_for_pipe(t_cmds *cmds)
   {
       // Case: | at start or end
       if (!cmds->prev || !cmds->next)
           return (errormsg_v2(cmds->cmd[0]));
       // Case: || (double pipe)
       if (cmds->next && cmds->next->token == Pipe)
           return (errormsg_v2(cmds->next->cmd[0]));
       return (0);
   }
   ```

2. **Redirection Errors**:
   ```c
   int check_for_in_out_put(t_cmds *cmds)
   {
       // Case: < or > without filename
       if (!cmds->next || (cmds->next->token == Non && !cmds->next->next))
           return (errormsg(" 'newline'\n"));
       // Case: < | or > | (redirection followed by pipe)
       else if (cmds->next && cmds->next->token == Pipe)
           return (errormsg_v2(cmds->next->cmd[0]));
       return (0);
   }
   ```

3. **Invalid Operator Sequences**:
   ```c
   int check_all_lt(t_cmds *cmds, int token_size)
   {
       // Detects <<<, <<<<, etc.
       int all_lt = 1;
       for (int i = 0; i < token_size && all_lt; i++) {
           if (cmds->cmd[0][i] != '<')
               all_lt = 0;
       }
       if (all_lt)
           return (print_all_lt_error(token_size));
       return (0);
   }
   ```

## Variable Expansion

### Expansion Process

```c
char *check_expand(char *str, t_data *data)
{
    // 1. Extract variable names from the string
    char **vars = get_vars(str);
    
    // 2. Get values for each variable
    char **var_values = get_vars_content(vars, data->env, str, 0);
    
    // 3. Replace variables with their values
    char *expanded = replace_variables_in_string(str, vars, var_values);
    
    return (expanded);
}
```

### Variable Types Handled

1. **Environment Variables**: `$HOME`, `$PATH`, etc.
2. **Exit Status**: `$?` - Last command exit code
3. **Process ID**: `$$` - Current shell PID
4. **Quoted Variables**: Variables inside single/double quotes

### Context-Aware Expansion

```c
char **get_vars_content(char **var, char **env, char *str, int i)
{
    char **vars = malloc(sizeof(char *) * (array_size(var) + 1));
    
    while (var[i]) {
        // Find $ position in original string
        while (str[k] && str[k] != '$') {
            k++;
            if (str[k] == '$' && str[k + 1] == '$')
                k += 2;  // Skip $$
        }
        
        if (var[i][0] == '?' && var[i][1] == '\0')
            vars[i] = ft_itoa(g_signal.ret);  // Exit status
        else if (check_back_for_heredoc(str, k) == true)
            vars[i] = ft_strjoin("$", var[i]);  // Don't expand in heredoc
        else
            vars[i] = get_env_variable_value(env, var[i]);  // Regular expansion
        i++;
        k++;
    }
    return (vars);
}
```

## Heredoc Processing

### Heredoc Flow

```c
int heredoc(t_cmds *head, t_cmds *curr, int i, int heredoc_num)
{
    // 1. Count and validate heredocs (max 4 per command)
    int status = count_and_validate_heredocs(curr, heredoc_num);
    if (status != 0)
        return (status);
        
    // 2. Process each heredoc delimiter
    return (process_heredoc_delimiters(head, i));
}
```

### Heredoc Implementation

```c
int open_heredoc(t_cmds *cmds, int pid, int status, bool flag)
{
    char *line;
    int fd0;
    
    // 1. Prepare temporary file and fork process
    pid = prepare_and_fork(cmds, &flag, &line, &fd0);
    
    if (pid == 0) {
        // Child process: read input until delimiter
        child(line, cmds, flag);
    } else if (pid < 0) {
        // Fork failed
        ft_putstr_fd("minishell: fork fail while creating the HereDoc\n", 2);
        return (-1);
    }
    
    // 2. Parent process: wait for child and update tokens
    status = fork_and_wait(pid, status, fd0, cmds->cmd[0]);
    cmds->cmd[0] = line;              // Replace delimiter with temp file
    cmds->prev->token = Input;        // Previous becomes input redirection
    cmds->token = Infile;             // This becomes input file
    
    return (status);
}
```

### Heredoc Child Process

```c
void child(char *line, t_cmds *cmds, bool flag)
{
    int fd = open(line, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    char *input_line;
    
    signal(SIGINT, signal_herd);  // Handle Ctrl+C
    
    while (1) {
        input_line = readline("> ");
        
        if (!input_line) {
            // EOF reached
            print_error(line_count, cmds->cmd[0]);
            break;
        }
        
        if (is_delimiter(input_line, cmds)) {
            // Delimiter found
            free(input_line);
            break;
        }
        
        // Write line to temporary file
        write_line(fd, input_line, cmds, flag);
        free(input_line);
    }
    
    close(fd);
    exit(0);
}
```

## Node Creation and Piping

### Command List Processing

```c
t_cmds *last_update_in_the_list(t_cmds *head, t_cmds *list, 
                                t_cmds *new_head_tmp, char **command)
{
    t_cmds *new_head;
    
    while (head) {
        list = head;
        
        // 1. Extract command from current pipeline segment
        if (get_cmd_size(list) == 0)
            command = get_name("");
        else {
            command = malloc(sizeof(char *) * (get_cmd_size(list) + 1));
            get_command_done(list, list, command, true);
        }
        
        // 2. Create new command node
        new_head = copy_list(list, command, 0);
        
        // 3. Merge with previous pipeline segments
        if (new_head_tmp)
            new_head = merge_lists(new_head_tmp, new_head);
            
        // 4. Find next pipeline segment (after pipe)
        while (list && list->token != Pipe)
            list = list->next;
            
        new_head_tmp = new_head;
        
        if (!list)
            break;
        head = list->next;
    }
    
    // Return to head of list
    while (new_head->prev)
        new_head = new_head->prev;
    return (new_head);
}
```

### Command Structure Creation

```c
t_command *get_commands(t_cmds *lst)
{
    t_command *commands = NULL;
    t_command *current_cmd;
    
    while (lst) {
        // Create new command node
        current_cmd = command_new(commands);
        
        // Extract command and arguments
        current_cmd->cmd = extract_command_args(lst);
        
        // Extract input files
        current_cmd->infile = extract_input_files(lst);
        
        // Extract output files  
        current_cmd->outfile = extract_output_files(lst);
        
        // Move to next pipeline
        lst = find_next_pipe_or_end(lst);
        
        if (!commands)
            commands = current_cmd;
    }
    
    return (commands);
}
```

## Detailed Example

Let's trace through a complex command: `echo "Hello $USER" | grep $USER > output.txt`

### Step 1: Initial Input
```
Input: echo "Hello $USER" | grep $USER > output.txt
```

### Step 2: Variable Expansion
```c
// check_expand() processes the string
// $USER is found and replaced with current username (e.g., "john")
Expanded: echo "Hello john" | grep john > output.txt
```

### Step 3: Tokenization (ft_split_msh)
```c
// Split into tokens while preserving quoted strings
Tokens: ["echo", "Hello john", "|", "grep", "john", ">", "output.txt"]
```

### Step 4: Linked List Creation
```c
// get_list() creates linked list nodes
Node 0: cmd=["echo"], token=Non
Node 1: cmd=["Hello john"], token=Non  
Node 2: cmd=["|"], token=Non
Node 3: cmd=["grep"], token=Non
Node 4: cmd=["john"], token=Non
Node 5: cmd=[">"], token=Non
Node 6: cmd=["output.txt"], token=Non
```

### Step 5: Token Assignment
```c
// init_tokens() assigns proper token types
Node 0: cmd=["echo"], token=Cmd
Node 1: cmd=["Hello john"], token=Non
Node 2: cmd=["|"], token=Pipe
Node 3: cmd=["grep"], token=Cmd
Node 4: cmd=["john"], token=Non
Node 5: cmd=[">"], token=Output
Node 6: cmd=["output.txt"], token=OutFile
```

### Step 6: Context Assignment
```c
// Context-aware assignment based on neighboring tokens
Node 0: cmd=["echo"], token=Cmd        // Command before pipe
Node 1: cmd=["Hello john"], token=Non  // Argument to echo
Node 2: cmd=["|"], token=Pipe          // Pipe operator
Node 3: cmd=["grep"], token=Cmd        // Command after pipe
Node 4: cmd=["john"], token=Non        // Argument to grep
Node 5: cmd=[">"], token=Output        // Output redirection
Node 6: cmd=["output.txt"], token=OutFile // Output file
```

### Step 7: Error Checking
```c
// errors_managment() validates syntax
- Check pipes: ✓ (has commands before and after)
- Check redirections: ✓ (has filename after >)
- Check heredocs: ✓ (none present)
```

### Step 8: Command Structure Creation
```c
// last_update_in_the_list() creates final command structure
Command 1:
  cmd: ["echo", "Hello john"]
  infile: NULL
  outfile: NULL
  next: Command 2

Command 2:
  cmd: ["grep", "john"]  
  infile: NULL
  outfile: [{"output.txt", Output}]
  next: NULL
```

### Step 9: Quote Removal
```c
// remove_quotes() processes quoted strings
Command 1: ["echo", "Hello john"]  // Quotes removed from "Hello john"
Command 2: ["grep", "john"]
```

### Final Structure Ready for Execution
The parsing is complete, and the command structure is ready for the execution engine.

## Function Reference

### Core Parsing Functions

| Function | Purpose | Input | Output |
|----------|---------|-------|---------|
| `parsing()` | Main parsing entry point | Raw input line | Parsed command structure |
| `prepare_line_and_tokens()` | Tokenize and validate | Expanded line | Token list |
| `init_tokens()` | Assign token types | Raw tokens | Typed tokens |
| `errors_managment()` | Syntax validation | Token list | Error status |

### Token Processing

| Function | Purpose | Input | Output |
|----------|---------|-------|---------|
| `assign_simple_tokens()` | Basic token assignment | Token string | Token type |
| `token1()` | Single-char operators | Operator char | Context-aware tokens |
| `token2()` | Double-char operators | Operator string | Context-aware tokens |
| `non_token()` | Default token assignment | Untyped nodes | Default tokens |

### Error Checking

| Function | Purpose | Detects |
|----------|---------|---------|
| `check_for_pipe()` | Pipe syntax errors | `\|`, `\|\|`, trailing pipes |
| `check_for_in_out_put()` | Redirection errors | `<`, `>` without files |
| `check_for_append_heredoc()` | Append/heredoc errors | `>>`, `<<` syntax |
| `check_all_lt()` | Invalid sequences | `<<<`, `<<<<`, etc. |

### Variable Expansion

| Function | Purpose | Input | Output |
|----------|---------|-------|---------|
| `check_expand()` | Main expansion function | Raw string | Expanded string |
| `get_vars()` | Extract variable names | String with $vars | Variable array |
| `get_vars_content()` | Get variable values | Variable names | Value array |
| `expand_variable()` | Single variable expansion | Variable name | Variable value |

### Heredoc Processing

| Function | Purpose | Input | Output |
|----------|---------|-------|---------|
| `heredoc()` | Main heredoc processor | Command list | Processed list |
| `open_heredoc()` | Create heredoc file | Delimiter | Temp file |
| `child()` | Heredoc input reader | Delimiter | Temp file content |
| `is_delimiter()` | Check end condition | Input line | Boolean |

### Command Structure

| Function | Purpose | Input | Output |
|----------|---------|-------|---------|
| `get_commands()` | Create command structure | Token list | Command list |
| `copy_list()` | Copy command segment | Token segment | Command node |
| `merge_lists()` | Merge command segments | Two lists | Merged list |
| `last_update_in_the_list()` | Final structure creation | Raw tokens | Final commands |

### Utility Functions

| Function | Purpose | Input | Output |
|----------|---------|-------|---------|
| `ft_split_msh()` | Minishell-aware split | Raw string | Token array |
| `rm_spaces()` | Remove extra whitespace | String | Trimmed string |
| `remove_quotes()` | Remove quote characters | Quoted strings | Unquoted strings |
| `cmdcheck()` | Check empty commands | Command string | Boolean |

This parsing system handles the complexity of shell syntax while maintaining clear separation of concerns and comprehensive error handling.
