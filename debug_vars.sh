#!/bin/bash

# Create a simple debug test to trace variable expansion
echo "=== Debugging Variable Expansion ==="

# Test a simple variable
echo "1. Testing \$HOME expansion:"
echo 'echo $HOME' > debug_test.txt
echo 'exit' >> debug_test.txt
echo "Input command: echo \$HOME"
./minishell < debug_test.txt

echo -e "\n2. Testing direct bash comparison:"
echo "Bash result: $(echo $HOME)"

echo -e "\n3. Testing \$USER expansion:"
echo 'echo $USER' > debug_test.txt
echo 'exit' >> debug_test.txt
echo "Input command: echo \$USER"
./minishell < debug_test.txt
echo "Bash result: $(echo $USER)"

# Clean up
rm -f debug_test.txt
