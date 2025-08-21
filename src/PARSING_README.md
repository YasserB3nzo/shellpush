# Minishell Parsing Pipeline

This document explains, step by step, how input from `readline` becomes executable commands in this project. It references concrete functions and files so you can jump to the implementation quickly.

## High-level flow

1. Read line: `readline("minishell$ ")` in `minishell.c`.
2. Validate quotes: `check_quotation` (`src/errors/errors0.c`).
3. Parse: `parsing(&data, NULL, NULL, -1)` (`src/parsing/parsing.c`).
4. Execute or report errors.

See the Mermaid diagram in `src/PARSING_FLOW.mmd` for a visual.

## Data structures (from `include/minishell.h`)

- `t_cmds`: Doubly-linked list of tokens/words with `token` and `operation` classifications.
- `t_command`: Final per-pipeline command node with `cmd`, `infile`, `outfile` lists.
- `t_slist`: Simple list for redirection files (in/out).
- `t_data`: Runtime context (`line`, `env`, `lst`, `list`, etc.).

## Step-by-step parsing

### 1) Entry and preprocessing (file: `src/parsing/parsing.c`)

- `parsing()` orchestrates the whole process.
- `get_cmds_done()` does two things:
  - `check_expand(data->line, data)` -> `expand_variable` if there are `$` occurrences.
  - `ft_split_msh(data->line)` to split into tokens (words and operators).
- For each token, if `cmdcheck(token) == 0` (i.e., it’s not only spaces), apply `rm_spaces` to trim outer spaces.
- Build the initial token list: `get_list(cmds, size, &lst, data)` -> creates a doubly linked `t_cmds` list.
- Classify tokens: `init_tokens(lst)` -> sets `token` and `operation` fields based on content/context.

### 2) Tokenization details

- `ft_split_msh` (`src/parsing/parsing_utils3.c`):
  - Uses `count_words`, `inside`, and `get_position` to count/split while respecting quotes.
  - Treats `|`, `<`, `>`, `<<`, `>>` as separate tokens unless inside quotes.
  - Allocates strings with `ndup`.
- `get_list` (`src/parsing/parsing_utils4.c`): builds `t_cmds` nodes from the split token array.
- `init_tokens` (`src/parsing/parsing_utils1.c`):
  - Single char: `<` => `Input`, `>` => `Output`, `|` => `Pipe`.
  - Double char: `<<` => `HereDoc`, `>>` => `Append`.
  - Neighbor-sensitive classification: marks neighbors as `Infile`, `OutFile`, `AppendFile`, `HereDocDel` when applicable; otherwise `Cmd`.
  - Sets `operation` to `Operation` for operators and `NonOperation` otherwise.

### 3) Expansion (`src/parsing/expand_variable.c` and helpers)

- `check_expand` -> `expand_variable` if any `$` present (`dollar_is_in`).
- `expand_variable`:
  - `get_vars` to extract variable names (see also `grep_variable_name`, `ft_split_str`, `count_vars`).
  - `get_vars_content` resolves values from `data->env` via `get_content` (`src/utils/utils5.c`). Special case: `$?` -> `g_signal.ret`; heredoc delimiters are not expanded.
  - Rebuilds the final expanded line: `get_final_line` using the literal segments and resolved values.
- Notes:
  - Expansion respects quotes: single quotes prevent `$` expansion; double quotes allow it. See `cmd_check` in `utils5.c`.

### 4) Error management (`src/errors/errors1.c`, `src/errors/errors0.c`)

- `errors_managment(data)` checks:
  - Heredocs via `heredoc`:
    - Count and limit (max 16).
    - For each `HereDocDel`, run `open_heredoc` (`src/parsing/open_heredoc.c`).
      - Spawns child, reads until delimiter, optionally expands, writes to `/tmp/HereDoc<N>`.
      - Rewrites tokens: previous becomes `Input`, current becomes `Infile`, content is the temp filename.
      - On Ctrl+C: exit status 130; parsing aborts for this line.
  - Pipes: `check_for_pipe` ensures no leading/trailing/adjacent pipes, proper neighbors.
  - Redirections: `check_for_in_out_put` and `check_for_append_heredoc` validate required filenames and operator placement.

If any check fails: `ft_clear(data)` is called and parsing returns without executing.

### 5) Quote removal (`src/parsing/parsing_utils5.c`)

- `remove_quotes(lst)` computes new string sizes via `get_size` and rebuilds strings without outer quotes using `get_string`.

### 6) Normalize tokens per pipeline (`src/parsing/parsing.c`)

- `last_update_in_the_list` re-materializes tokens per pipe segment:
  - `get_command_done` composes argv-like chunks for commands.
  - `copy_list`/`merge_lists` to rebuild a normalized `t_cmds` list with correct grouping.
- Clear and replace `data->lst` with the normalized list.

### 7) Build executable commands (`src/parsing/parsing_utils5.c`, `src/parsing/parsing_utils8.c`)

- Allocate `t_command` chain with `get_command` (one per pipe segment).
- Populate each `t_command` via `fill_in_commands`:
  - First `Cmd`/`Non` sequence -> `command->cmd` (argv array).
  - `Infile` nodes -> `command->infile` list (`t_slist`).
  - `OutFile`/`AppendFile` nodes -> `command->outfile` list (`t_slist`).
- `get_commands` returns the head that is stored in `data->list`.

### 8) Execution handoff

- `executing(data)` consumes `data->list`.
- After execution: `commands_clear`, `ft_clear` cleanup structures.

## Edge cases covered

- Unbalanced quotes -> immediate error (`check_quotation`).
- Consecutive pipes or pipe at ends -> error (`check_for_pipe`).
- Redirection without filename or misplaced operators -> error helpers.
- Heredoc signals and empty delimiters handled; temp file path persisted.
- Variable expansion disabled in single quotes and after heredoc delimiters.

## Visual diagram

Open `src/PARSING_FLOW.mmd` in a Mermaid preview to see the complete flow.

## Functions and call flow

This section enumerates the major functions in order of invocation with file references.

1) Input and guardrails
- `main` (minishell.c) -> calls `signal_handler`, builds `data.env`, reads `data.line` via `readline`, checks `check_quotation` (errors0.c), then calls `parsing`.

2) Parsing orchestrator (src/parsing/parsing.c)
- `parsing(data, lst, commands, i)`
  - `get_cmds_done`:
    - `check_expand` (expand_variable.c)
      - `expand_variable`
        - `dollar_is_in` (parsing_utils6.c)
        - `get_vars` (prototype in header; see helpers `grep_variable_name`, `ft_split_str`, `count_vars`)
        - `get_vars_content` (expand_variable.c) -> `get_content` (utils5.c)
        - `get_final_line` (utils5.c)
    - `ft_split_msh` (parsing_utils3.c)
      - `count_words` (parsing_utils4.c)
      - `inside` and `get_position` (parsing_utils3.c)
  - For each split token: `cmdcheck` (errors1.c); if not only spaces -> `rm_spaces` (parsing_utils2.c)
  - `get_list` (parsing_utils4.c) builds `t_cmds`
  - `init_tokens` (parsing_utils1.c) -> `non_token`
  - `errors_managment` (errors1.c)
    - `heredoc` -> `open_heredoc` (open_heredoc.c)
      - `check_quot_and_filename`, `ft_fork`, child loop with `expand_variable`
    - `check_for_pipe` (errors0.c)
    - `check_for_in_out_put` / `check_for_append_heredoc` (parsing_utils2.c)
  - `remove_quotes` (parsing_utils5.c)
  - `last_update_in_the_list` (parsing.c)
    - `get_cmd_size`, `get_command_done` (utils0.c)
    - `copy_list`, `merge_lists` (utils9.c)
  - Build final commands:
    - `get_commands` (parsing_utils8.c)
      - `get_command` (parsing_utils5.c)
      - `fill_in_commands` (parsing_utils8.c)
  - `executing` (src/execution/executing.c) then cleanup: `commands_clear`, `ft_clear`.

3) Token types and semantics
- Enum `t_token` (include/minishell.h): `Cmd`, `Infile`, `OutFile`, `AppendFile`, `HereDocDel`, operators `Input`, `Output`, `Append`, `HereDoc`, `Pipe`, plus `Non` and `Operation/NonOperation` markers in `t_cmds`.
- `init_tokens` decides immediate neighbors’ roles and marks operator vs non-operator in `operation`.

4) Redirection and heredoc specifics
- `fill_in_commands` attaches `Infile` and `Outfile`/`AppendFile` entries per `t_command`.
- `open_heredoc` writes the here-doc body to a tmp file, converts tokens: previous becomes `Input`, current becomes `Infile`, and the argument becomes the tmp filename.

5) Expansion rules
- `$?` becomes `g_signal.ret`.
- Variables resolved from `data->env` using `get_content`.
- Inside single quotes: no expansion. Inside double quotes: expansion allowed.
- After a heredoc delimiter (quoted), expansion is disabled.

For a compact function-call diagram, see the "Function Flow" subgraph in `PARSING_FLOW.mmd`.
