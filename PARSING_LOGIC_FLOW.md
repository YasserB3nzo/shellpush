# Minishell Parsing Logic Flow - Step by Step Explanation

This document provides a detailed, step-by-step explanation of how the parsing works in this minishell implementation. We will trace through the entire process from receiving user input to generating executable commands, explaining why each step happens and what each function does.

## Overview: The Complete Flow

When a user types something like `echo "Hello $USER" | grep Hello > output.txt`, here's what happens:

1. **Input Reading** (minishell.c) - Read the line via readline
2. **Quote Validation** (errors/shellerr.c) - Check if quotes are balanced
3. **Variable Expansion** (parsing/expander.c) - Expand $variables
4. **Tokenization** (parsing/parse3.c) - Split into words and operators
5. **List Building** (parsing/parse4.c) - Create doubly-linked list
6. **Token Classification** (parsing/parse1.c) - Identify token types
7. **Error Checking** (errors/shellerr.c) - Validate syntax
8. **Quote Removal** (parsing/parse5.c) - Remove quotes from tokens
9. **Command Building** (parsing/parse8.c) - Build final command structures
10. **Execution Handoff** (execution/exec.c) - Pass to execution engine

Now let's trace through each step in detail:

---

## Step 1: Input Reading and Initial Setup (minishell.c)

### Function: `main()` → `parsing()`

**What happens:**
```c
data.line = readline("minishell$ ");
if (check_quotation(data.line) != -1) {
    add_history(data.line);
    parsing(&data, NULL, NULL, -1);
}
```

**Why this step:**
- We first read user input using readline library
- We check if quotes are balanced before proceeding (because unbalanced quotes are syntax errors)
- If quotes are valid, we add to history and call the main parsing orchestrator

**Example:** User types: `echo "Hello $USER" | grep Hello`
- `data.line` now contains: `"echo \"Hello $USER\" | grep Hello"`

---

## Step 2: Main Parsing Orchestrator (src/parsing/parser.c)

### Function: `parsing(t_data *data, t_cmds *lst, t_command *commands, int i)`

**What happens first:**
```c
data->cmds = get_cmds_done(data, data->cmds);
```

This calls `get_cmds_done()` which does two critical things:

1. **Variable Expansion**: `data->line = check_expand(data->line, data)`
2. **Tokenization**: `cmds = ft_split_msh(data->line)`

**Why this order:** We must expand variables BEFORE splitting into tokens, because variable expansion can change the structure of the command.

---

## Step 3: Variable Expansion (src/parsing/expander.c)

### Function: `check_expand()` → `expand_variable()`

**What happens:**
```c
if (count_dollar_signs(str)) {
    var = get_vars(str);                    // Extract variable names
    var = get_vars_content(var, data->env, str, 0); // Get their values
    spleted_line = ft_split_str(str);       // Split literal parts
    line = get_final_line(spleted_line, var, str, &line_data); // Rebuild
}
```

**Why we do this:**
- Variables like `$USER`, `$HOME`, `$?` need to be replaced with their actual values
- Special case: `$?` expands to the exit status of the last command
- Heredoc delimiters are NOT expanded (checked via `check_back_for_heredoc()`)

**Example:** 
- Input: `echo "Hello $USER"`
- If `$USER = "john"`, becomes: `echo "Hello john"`

**Key functions:**
- `get_vars()`: Extracts variable names (USER from $USER)
- `get_vars_content()`: Looks up values in environment
- `get_final_line()`: Reassembles the string with expanded values

---

## Step 4: Tokenization (src/parsing/parse3.c)

### Function: `ft_split_msh(char const *s)`

**What happens:**
```c
word_count = count_words(s, 0, 0) - inside(s);  // Count tokens, minus operators inside quotes
ptr = malloc((word_count + 1) * sizeof(char *));
ptr = split(s, 0, ptr, 1);                      // Actually split the string
```

**Why this algorithm:**
- Shell operators (`|`, `<`, `>`, `<<`, `>>`) must be separate tokens unless inside quotes
- `inside()` counts operators that are inside quotes (so they're NOT split)
- `count_words()` counts all potential tokens
- `split()` does the actual work, using `get_position()` to respect quotes

**Example:**
- Input: `echo "Hello john" | grep Hello`
- Tokens: `["echo", "\"Hello john\"", "|", "grep", "Hello"]`

**Key insight:** Quotes are preserved at this stage - they'll be removed later.

---

## Step 5: Building the Token List (src/parsing/parse4.c)

### Function: `get_list(char **cmd, int size, t_cmds **lst, t_data *data)`

**What happens:**
```c
*lst = lstnew(cmd[i++], *lst, NULL);    // Create first node
(*lst)->data = data;                    // Link to data context
while (i < size) {
    node = lstnew(cmd[i], *lst, NULL);  // Create subsequent nodes
    node->data = data;
    curr = lstlast(*lst);
    curr->next = node;                  // Link them together
    i++;
}
```

**Why doubly-linked list:**
- Each token needs to know its neighbors for classification
- We need to traverse forward and backward during parsing
- Easy insertion/deletion during processing

**Result:** Each token is now a `t_cmds` node with:
- `cmd`: Array of words (split by space)
- `token`: Will be set in next step
- `prev`/`next`: Links to neighbors

---

## Step 6: Token Classification (src/parsing/parse1.c)

### Function: `init_tokens(t_cmds *cmds, int token_size, t_cmds *lst)`

This is where the magic happens - we determine what each token represents.

**What happens:**
```c
while (cmds) {
    token_size = ft_strlen(cmds->cmd[0]);
    if (token_size == 1 && cmds->cmd[0][0] == '<')
        token1(cmds, '<');              // Input redirection
    else if (token_size == 1 && cmds->cmd[0][0] == '>')
        token1(cmds, '>');              // Output redirection
    else if (token_size == 1 && cmds->cmd[0][0] == '|')
        token1(cmds, '|');              // Pipe
    else if (token_size == 2 && cmds->cmd[0][0] == '>' && cmds->cmd[0][1] == '>')
        token2(cmds, 1);                // Append redirection
    else if (token_size == 2 && cmds->cmd[0][0] == '<' && cmds->cmd[0][1] == '<')
        token2(cmds, 2);                // Heredoc
    // ... handle other cases
}
```

**The classification logic (token1 and token2 functions):**

For `<` (Input):
```c
if (cmds->prev && cmds->prev->token == Non)
    cmds->prev->token = Cmd;           // Previous token is a command
if (cmds->next && cmds->next->token == Non)
    cmds->next->token = Infile;        // Next token is input filename
cmds->token = Input;                   // This token is input operator
```

**Why this neighbor-based approach:**
- Context determines meaning: `cat < file.txt` - "cat" is command, "file.txt" is infile
- We need to look at neighbors to properly classify tokens
- Same word can be different things: "echo" could be command or filename

**Example classification:**
- `echo "Hello" | grep Hello > output.txt`
- `echo`: Cmd, `"Hello"`: Cmd (argument), `|`: Pipe, `grep`: Cmd, `Hello`: Cmd, `>`: Output, `output.txt`: OutFile

---

## Step 7: Error Management (src/errors/shellerr.c)

### Function: `errors_managment(t_data *data, int flag)`

**What happens:**
```c
i = heredoc(head, head, i, 0);          // Check heredoc limits and process them
while (curr && i == 0) {
    if (curr->token == Pipe)
        i = check_for_pipe(curr);       // Validate pipe syntax
    else if (curr->token == Output || curr->token == Input)
        i = check_for_in_out_put(curr); // Validate redirection syntax
    else if (curr->token == Append || curr->token == HereDoc)
        i = check_for_append_heredoc(curr);
    curr = curr->next;
}
```

**Why validate now:**
- Syntax errors must be caught before execution
- Different rules for different operators (pipes can't be at start/end, redirections need filenames)
- Heredocs are limited to 16 per command (bash limitation)

**Common errors caught:**
- `| echo hello` (pipe at start)
- `echo hello >` (redirection without filename)
- `echo hello | |` (consecutive pipes)

---

## Step 8: Quote Removal (src/parsing/parse5.c)

### Function: `remove_quotes(t_cmds *lst)`

**What happens:**
```c
while (lst) {
    i = 0;
    while (lst->cmd[i]) {
        if (lst->cmd[i] != NULL && get_size(lst->cmd[i]) != ft_strlen(lst->cmd[i])) {
            lst->cmd[i] = get_string(lst->cmd[i], 0, 0, get_size(lst->cmd[i]));
        }
        i++;
    }
    lst = lst->next;
}
```

**Why remove quotes now:**
- Quotes were needed during parsing to preserve structure
- Execution needs the actual content without quotes
- `get_size()` calculates size without quotes
- `get_string()` extracts content, handling nested quotes

**Example:**
- Before: `"Hello john"`
- After: `Hello john`

---

## Step 9: List Normalization (src/parsing/parser.c)

### Function: `last_update_in_the_list()`

**What happens:**
This complex function processes each pipeline (sequence of commands separated by pipes):

```c
while (head) {
    list = head;
    if (get_cmd_size(list) == 0)
        command = get_name("");         // Empty command
    else {
        command = malloc(sizeof(char *) * (get_cmd_size(list) + 1));
        get_command_done(list, list, command, true); // Collect all args for this command
    }
    new_head = copy_list(list, command, 0);
    // ... merge with previous pipeline parts
    while (list && list->token != Pipe)
        list = list->next;              // Skip to next pipeline
}
```

**Why this step:**
- Commands and their arguments need to be grouped together
- Multiple tokens like `ls -la /home` become one command with arguments
- Separates different pipelines for independent processing

---

## Step 10: Final Command Structure Building (src/parsing/parse8.c)

### Function: `get_commands(t_cmds *lst)`

This is the final transformation - converting our token list into executable command structures.

**What happens:**
```c
command = get_command(lst);             // Create command structure
head = command;
while (command && lst) {
    infile = NULL;
    outfile = NULL;
    while (lst && lst->token != Pipe) { // Process one pipeline
        fill_in_commands(&lst, &command, &infile, &outfile);
        lst = lst->next;
    }
    command->infile = get_head(infile);
    command->outfile = get_head(outfile);
    // Move to next command in pipeline
}
```

**The `fill_in_commands()` logic:**
```c
if ((*cl)->cmd && !(*command)->cmd && ((*cl)->token == Cmd || (*cl)->token == Non))
    (*command)->cmd = array_copy((*cl)->cmd);     // This is the main command

else if ((*cl)->cmd && (*cl)->token == Infile) {
    // Add to input file list
    if (!(*infile))
        (*infile) = node_new(*infile, (*cl)->cmd[0], (*cl)->token);
    else {
        (*infile)->next = node_new(*infile, (*cl)->cmd[0], (*cl)->token);
        *infile = (*infile)->next;
    }
}

else if ((*cl)->cmd[0] && ((*cl)->token == OutFile || (*cl)->token == AppendFile)) {
    // Add to output file list
    // Similar logic for output files
}
```

**Why this structure:**
- Each command needs its arguments in one array
- Input/output redirections are separate lists (multiple redirections possible)
- Pipeline commands are linked together

**Final result for `echo "Hello" | grep Hello > output.txt`:**

Command 1:
- `cmd`: `["echo", "Hello", NULL]`
- `infile`: NULL
- `outfile`: NULL
- `next`: → Command 2

Command 2:
- `cmd`: `["grep", "Hello", NULL]`
- `infile`: NULL
- `outfile`: → `{cmd: "output.txt", token: OutFile}`
- `next`: NULL

---

## Step 11: Execution Handoff

Finally, the parsing is complete and we hand off to the execution engine:

```c
commands = get_commands(lst);
data->list = commands;
g_signal.ret = executing(data);      // Execute the commands
```

---

## Why This Parsing Approach?

This multi-stage approach might seem complex, but each stage has a specific purpose:

1. **Early variable expansion** ensures variables are resolved before structure analysis
2. **Quote-aware tokenization** preserves user intent while splitting
3. **Neighbor-based classification** handles context-dependent meanings
4. **Error checking after classification** can provide precise error messages
5. **Late quote removal** ensures quotes don't interfere with parsing logic
6. **Final structure building** creates clean, execution-ready commands

## Key Data Structure Transformations

1. **String** → **Token Array** (ft_split_msh)
2. **Token Array** → **Linked List** (get_list)
3. **Linked List** → **Classified List** (init_tokens)
4. **Classified List** → **Command Structures** (get_commands)

Each transformation adds more semantic meaning while preserving all necessary information for execution.

This design makes the parser robust and maintainable, handling complex shell syntax while providing clear error messages and preparing data structures optimized for execution.

---

## Appendix: Detailed Examples and Edge Cases

### Example 1: Complex Command with Multiple Redirections

**Input:** `cat < input.txt | grep "pattern" >> output.txt 2>&1`

**Step-by-step breakdown:**

1. **After tokenization:** `["cat", "<", "input.txt", "|", "grep", "\"pattern\"", ">>", "output.txt", "2>&1"]`

2. **After classification:**
   - `cat`: Cmd
   - `<`: Input  
   - `input.txt`: Infile
   - `|`: Pipe
   - `grep`: Cmd
   - `"pattern"`: Cmd (argument)
   - `>>`: Append
   - `output.txt`: AppendFile
   - `2>&1`: Cmd (treated as argument - stderr redirection handled by execution)

3. **Final command structure:**
   - Command 1: `cmd=["cat"], infile=["input.txt"], outfile=NULL`
   - Command 2: `cmd=["grep", "pattern", "2>&1"], infile=NULL, outfile=["output.txt" (append)]`

### Example 2: Variable Expansion Edge Cases

**Input:** `echo "$USER_$HOME_$?"` (where USER=john, HOME=/home/john, ?=0)

**Expansion process:**
1. `get_vars()` extracts: `["USER_", "HOME_", "?"]`
2. `get_vars_content()` resolves: `["john", "/home/john", "0"]`
3. `get_final_line()` produces: `echo "john/home/john0"`

**Why the underscore handling:** Variable names can contain underscores, so `$USER_` is treated as variable `USER_`, not `USER` + `_`.

### Example 3: Quote Handling

**Input:** `echo 'Don'\''t expand $USER' "but expand $USER here"`

**Processing:**
1. **Tokenization preserves quotes:** `["echo", "'Don'\\''t expand $USER'", "\"but expand $USER here\""]`
2. **Expansion only affects double quotes:** `["echo", "'Don'\\''t expand $USER'", "\"but expand john here\""]`
3. **Quote removal:** `["echo", "Don't expand $USER", "but expand john here"]`

### Example 4: Heredoc Processing

**Input:**
```bash
cat << EOF
Hello $USER
EOF
```

**Special handling:**
- The delimiter `EOF` is NOT subject to variable expansion
- Content between delimiters IS subject to expansion (unless delimiter is quoted)
- `check_back_for_heredoc()` prevents delimiter expansion

### Example 5: Error Detection

**Input:** `echo hello | | grep world`

**Error detection in `errors_managment()`:**
- First `|` is classified as Pipe
- `check_for_pipe()` sees next token is also `|` (Pipe)
- Returns syntax error: "syntax error near unexpected token `|'"

---

## Appendix: Key Helper Functions

### Memory Management
- `free_array()`: Safely frees string arrays
- `lstclear()`: Frees linked list nodes
- `commands_clear()`: Frees command structures

### String Utilities  
- `ndup()`: Duplicates n characters from string
- `rm_spaces()`: Removes leading/trailing whitespace
- `get_string()`: Extracts content from quoted strings

### List Operations
- `lstnew()`: Creates new list node
- `lstlast()`: Gets last node in list  
- `get_head()`: Gets first node in list

### Token Classification Helpers
- `cmdcheck()`: Checks if string is only whitespace
- `is_spaces()`: Validates spacing
- `count_words()`: Counts tokens while respecting quotes

### Variable Processing
- `dollar_is_in()`: Checks if string contains variables
- `extract_variable_name()`: Extracts variable name after $
- `get_env_variable_value()`: Looks up environment variable value

---

## Appendix: Common Parsing Challenges and Solutions

### Challenge 1: Quote Nesting
**Problem:** `echo "He said 'Hello' to me"`
**Solution:** `get_string()` tracks quote state and handles nested quotes correctly

### Challenge 2: Operator Disambiguation  
**Problem:** Is `>>` two `>` operators or one `>>` operator?
**Solution:** `count_words()` and `split()` specifically check for double-character operators first

### Challenge 3: Variable Boundary Detection
**Problem:** In `$USER_NAME`, is the variable `$USER` or `$USER_NAME`?
**Solution:** `extract_variable_name()` uses `ft_isalnum()` to find the longest valid variable name

### Challenge 4: Pipeline Boundary Detection
**Problem:** Where does one command end and the next begin in a pipeline?
**Solution:** `get_commands()` processes tokens until hitting a Pipe token, then starts a new command

### Challenge 5: Redirection Accumulation
**Problem:** `cat < file1 < file2 > out1 > out2`
**Solution:** `fill_in_commands()` builds linked lists of input/output files, preserving order

---

## Appendix: Performance Considerations

### Why Multiple Passes?
- **Single-pass parsing** would be more complex and error-prone
- **Multi-pass approach** allows each stage to focus on one concern
- **Memory overhead** is acceptable for typical shell command lengths
- **Clarity and maintainability** outweigh performance in interactive shell

### Memory Management Strategy
- **Immediate cleanup** after each stage where possible
- **Reference counting** avoided in favor of clear ownership
- **Error paths** always clean up allocated memory
- **Linked lists** allow dynamic sizing without pre-allocation

### Optimization Opportunities
- **Token reuse** could eliminate some string copying
- **In-place quote removal** could reduce allocations
- **Pooled allocation** could reduce malloc overhead
- **Lazy expansion** could defer variable resolution

However, these optimizations are not implemented because:
1. Interactive shell performance is not bottlenecked by parsing
2. Code clarity is more important than micro-optimizations
3. Memory usage is already reasonable for typical commands