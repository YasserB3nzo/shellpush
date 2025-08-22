#!/bin/bash

echo "Testing hypothesis about ft_split_str..."

# Test simple variable expansion that might not get split
echo "=== Test 1: Simple variable (might use join_vars) ==="
echo 'echo $USER' | timeout 5s ./minishell 2>&1

echo -e "\n=== Test 2: Variable with text (definitely uses get_final_line) ==="
echo 'echo Hello$USER' | timeout 5s ./minishell 2>&1

echo -e "\n=== Test 3: Heredoc variable expansion ==="
cat << 'EOF' > heredoc_var_test.txt
cat << END
$USER
END
exit
EOF
timeout 5s ./minishell < heredoc_var_test.txt 2>&1

rm -f heredoc_var_test.txt
