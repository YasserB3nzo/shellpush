# 42 MINISHELL MANUAL TESTING GUIDE

This guide covers manual tests that are difficult to automate but essential for 42 minishell compliance.

## SIGNAL HANDLING (CRITICAL for 42)

### Test 1: Ctrl+C (SIGINT) Behavior
1. Run `./minishell`
2. Type `sleep 10` and press Enter
3. While sleep is running, press `Ctrl+C`
4. **Expected**: 
   - Sleep process terminates
   - New prompt appears immediately
   - No "^C" printed
   - Exit status should be 130: `echo $?` after exiting

### Test 2: Ctrl+C at Empty Prompt  
1. Run `./minishell`
2. Press `Ctrl+C` at empty prompt
3. **Expected**:
   - New line and fresh prompt
   - Shell doesn't exit
   - Exit status 130

### Test 3: Ctrl+D (EOF) Behavior
1. Run `./minishell` 
2. Press `Ctrl+D` at empty prompt
3. **Expected**:
   - Prints "exit"
   - Shell terminates cleanly
   - Returns to parent shell

### Test 4: Ctrl+\ (SIGQUIT) Behavior
1. Run `./minishell`
2. Type `sleep 10` and press Enter
3. Press `Ctrl+\`
4. **Expected**:
   - Should be ignored (no effect)
   - Sleep continues running

## HEREDOC FUNCTIONALITY

### Test 1: Basic Heredoc
```bash
./minishell
cat << EOF
line 1
line 2
EOF
```
**Expected**: Outputs both lines

### Test 2: Heredoc with Variable Expansion
```bash
./minishell
export USER=testuser
cat << EOF  
Hello $USER
Your home is $HOME
EOF
```
**Expected**: Variables are expanded

### Test 3: Heredoc with Quotes (No Expansion)
```bash
./minishell
cat << 'EOF'
Hello $USER
EOF
```
**Expected**: $USER printed literally

### Test 4: Multiple Heredocs
```bash
./minishell  
cat << EOF1 << EOF2
first
EOF1
second  
EOF2
```

### Test 5: Heredoc with Ctrl+C
1. Start: `cat << EOF`
2. Press Ctrl+C while waiting for input
3. **Expected**: Cancels heredoc, returns to prompt

## ENVIRONMENT VARIABLE HANDLING

### Test 1: Exit Status Variable
```bash
./minishell
true
echo $?          # Should print 0
false  
echo $?          # Should print 1
ls /nonexistent
echo $?          # Should print 2 or 127
```

### Test 2: Variable Export/Inheritance
```bash
./minishell
export TEST_VAR="hello world"
bash             # Start a child bash
echo $TEST_VAR   # Should print "hello world"
exit
echo $TEST_VAR   # Should still work in minishell
```

### Test 3: PATH Variable Handling
```bash
./minishell  
echo $PATH
export PATH=""
ls            # Should fail with command not found
export PATH="/bin:/usr/bin"
ls            # Should work again
```

## QUOTE PARSING

### Test 1: Complex Quote Combinations
```bash
./minishell
echo "Hello 'world' test"
echo 'Hello "world" test'  
echo "Hello $USER 'test'"
echo 'No expansion: $HOME'
```

### Test 2: Empty Quotes
```bash
./minishell
echo "" '' "test"
echo '''''' 
```

### Test 3: Quotes with Pipes and Redirections
```bash
./minishell
echo "hello | world" > test.txt
cat test.txt       # Should contain: hello | world
echo 'test > file'  # Should print: test > file
```

## PIPE FUNCTIONALITY  

### Test 1: Long Pipe Chains
```bash
./minishell
echo "hello world test" | tr ' ' '\n' | sort | uniq | wc -l
```

### Test 2: Pipes with Built-ins
```bash
./minishell
export | grep PATH | head -5
env | sort | tail -10
```

### Test 3: Failed Commands in Pipes
```bash
./minishell
nonexistent | cat
echo $?              # Should be 127
```

## REDIRECTION EDGE CASES

### Test 1: Multiple Redirections
```bash
./minishell
echo "test" > file1.txt > file2.txt  
# Both files should exist, file2.txt should contain "test"
```

### Test 2: Permission Errors
```bash
./minishell
touch readonly.txt
chmod 000 readonly.txt  
echo "test" > readonly.txt  # Should fail
echo $?                     # Should be non-zero
```

### Test 3: Append vs Overwrite
```bash
./minishell
echo "line1" > test.txt
echo "line2" >> test.txt
cat test.txt         # Should show both lines
echo "line3" > test.txt  
cat test.txt         # Should show only line3
```

## BUILTIN COMMANDS COMPLIANCE

### Test 1: CD with Various Arguments
```bash
./minishell
cd                 # Should go to $HOME
cd /               # Should go to root
cd .               # Should stay in place  
cd ..              # Should go to parent
cd -               # Should go to previous directory
cd ~               # Should go to home
cd ~/Desktop       # Should work if Desktop exists
```

### Test 2: Export Edge Cases
```bash
./minishell
export             # Should list all variables
export VAR         # Creates empty variable
export VAR=        # Creates empty variable  
export VAR=value   # Sets variable
export =value      # Should fail
export 123VAR=test # Should fail
```

### Test 3: Echo Flag Parsing
```bash
./minishell
echo -n hello
echo -nnnn hello  
echo -n -n hello
echo -- -n hello   # -n should be treated as text
```

### Test 4: Exit with Various Arguments
```bash
./minishell
exit               # Should exit with last command status
exit 42            # Should exit with status 42
exit abc           # Should print error, exit with status 2
exit 999999999999  # Should handle overflow properly
```

## MEMORY MANAGEMENT

### Test 1: Long Session Test
Run a long session with many commands to check for memory leaks:
```bash
./minishell
# Then run 100+ commands of various types
# Monitor with: valgrind --leak-check=full ./minishell
```

### Test 2: Large Command Lines  
```bash
./minishell
echo $(python3 -c "print('a' * 1000)")
```

## NORMINETTE COMPLIANCE

Run these checks:
```bash
norminette .
make fclean && make  # Should compile without warnings
```

## FINAL INTEGRATION TESTS

### Test 1: Complete Feature Integration
```bash
./minishell
export MY_PATH="/tmp/test"
mkdir -p $MY_PATH  
cd $MY_PATH
echo "Hello World" > greeting.txt
cat greeting.txt | tr 'a-z' 'A-Z' > upper.txt
if [ -f upper.txt ]; then echo "Success"; fi
cd -
rm -rf $MY_PATH
exit 42
```
Then check: `echo $?` should be 42

### Test 2: Bash Comparison
For any complex command, compare:
```bash
# In bash:
echo "complex command here" 

# In minishell:
echo "complex command here"

# Results should be identical
```

## EXPECTED BEHAVIORS (42 Requirements)

1. **Prompt**: Should show `minishell$ ` (or custom prompt)
2. **History**: Up/down arrows should work  
3. **Signals**: Ctrl+C, Ctrl+D, Ctrl+\ handled like bash
4. **Exit Status**: Always preserve and return correct exit status
5. **Built-ins**: cd, echo, env, exit, export, pwd, unset must work
6. **No Memory Leaks**: Use valgrind to verify
7. **Norm Compliance**: Must pass norminette  
8. **Error Messages**: Should match bash error format when possible

## DEBUGGING TIPS

If tests fail:
1. Compare with bash: `echo "command" | bash` vs `echo "command" | ./minishell`
2. Check exit status: `echo $?` after each command
3. Use valgrind: `valgrind ./minishell` for memory issues
4. Enable debug output in your code if needed
5. Test each feature in isolation first

Remember: The goal is to mimic bash behavior as closely as possible within the 42 project constraints!
