#!/bin/bash

# Quick test to verify variable expansion fix
echo "Testing variable expansion fix..."

echo 'echo $USER' | timeout 5s ./minishell > test_var_output.txt 2>&1
echo 'exit' | timeout 5s ./minishell >> test_var_output.txt 2>&1

echo "=== Variable Expansion Test Output ==="
cat test_var_output.txt

echo -e "\n=== Testing $? variable ==="
echo 'false' > test_exit_status.txt
echo 'echo $?' >> test_exit_status.txt
echo 'exit' >> test_exit_status.txt

timeout 5s ./minishell < test_exit_status.txt > test_exit_output.txt 2>&1
cat test_exit_output.txt

echo -e "\n=== Testing empty quotes ==="
echo 'echo ""' | timeout 5s ./minishell > test_empty_quotes.txt 2>&1
echo 'exit' | timeout 5s ./minishell >> test_empty_quotes.txt 2>&1
cat test_empty_quotes.txt

# Clean up
rm -f test_var_output.txt test_exit_status.txt test_exit_output.txt test_empty_quotes.txt
