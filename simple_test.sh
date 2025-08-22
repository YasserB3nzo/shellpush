#!/bin/bash

# Simplified test focusing on basic functionality
echo "=== TESTING BASIC MINISHELL FUNCTIONALITY ==="

# Test 1: Basic echo
echo "Test 1: Basic echo"
echo "echo hello" | ./minishell | head -n 1

# Test 2: Environment variable
echo -e "\nTest 2: Environment variable"
echo "echo \$HOME" | ./minishell | head -n 1

# Test 3: PWD
echo -e "\nTest 3: PWD command"
echo "pwd" | ./minishell | head -n 1

# Test 4: Simple pipe
echo -e "\nTest 4: Simple pipe"
echo "echo hello | cat" | ./minishell | head -n 1

# Test 5: Input redirection
echo "test content" > /tmp/test_input.txt
echo -e "\nTest 5: Input redirection"
echo "cat < /tmp/test_input.txt" | ./minishell | head -n 1

# Test 6: Output redirection
echo -e "\nTest 6: Output redirection"
echo "echo hello > /tmp/test_output.txt" | ./minishell
echo "File contents: $(cat /tmp/test_output.txt 2>/dev/null || echo 'FAILED')"

# Test 7: Exit code
echo -e "\nTest 7: Exit with custom code"
echo "exit 42" | ./minishell
echo "Exit code was: $?"

# Cleanup
rm -f /tmp/test_*.txt

echo -e "\n=== TESTING COMPLETE ==="
