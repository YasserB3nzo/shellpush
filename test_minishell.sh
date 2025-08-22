#!/bin/bash

# Minishell Test Suite - Based on 42 Evaluation Criteria
# This script tests all required functionality of minishell

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local input="$2"
    local expected_output="$3"
    local expected_exit_code="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}[TEST $TOTAL_TESTS]${NC} $test_name"
    echo -e "${YELLOW}Input:${NC} $input"
    echo -e "${YELLOW}Expected:${NC} $expected_output (exit: $expected_exit_code)"
    
    # Run the command in minishell
    result=$(echo "$input" | timeout 5s ./minishell 2>&1 | tail -n +2)
    exit_code=$?
    
    echo -e "${YELLOW}Got:${NC} $result (exit: $exit_code)"
    
    # Check result
    if [[ "$result" == *"$expected_output"* ]] && [[ "$exit_code" == "$expected_exit_code" ]]; then
        echo -e "${GREEN}✓ PASSED${NC}\n"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}✗ FAILED${NC}\n"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Function to run interactive test
run_interactive_test() {
    local test_name="$1"
    local commands="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}[INTERACTIVE TEST $TOTAL_TESTS]${NC} $test_name"
    echo -e "${YELLOW}Commands:${NC} $commands"
    
    # Create temporary script
    echo "$commands" > /tmp/minishell_test.sh
    chmod +x /tmp/minishell_test.sh
    
    # Run the test
    result=$(timeout 10s /tmp/minishell_test.sh 2>&1)
    exit_code=$?
    
    echo -e "${YELLOW}Output:${NC} $result"
    
    if [[ $exit_code -eq 0 ]]; then
        echo -e "${GREEN}✓ PASSED${NC}\n"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAILED${NC}\n"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    
    rm -f /tmp/minishell_test.sh
}

echo -e "${BLUE}=== MINISHELL COMPREHENSIVE TEST SUITE ===${NC}\n"

# 1. BASIC COMMAND EXECUTION TESTS
echo -e "${BLUE}=== 1. BASIC COMMAND EXECUTION ===${NC}"

run_test "Simple echo" "echo hello" "hello" 0
run_test "Echo with arguments" "echo hello world" "hello world" 0
run_test "PWD command" "pwd" "$(pwd)" 0
run_test "LS command" "ls" "" 0  # Just check it doesn't crash
run_test "Environment variable" "echo \$USER" "$USER" 0

# 2. BUILTIN COMMANDS TESTS
echo -e "${BLUE}=== 2. BUILTIN COMMANDS ===${NC}"

run_test "CD to home" "cd ~" "" 0
run_test "CD to root" "cd /" "" 0
run_test "PWD after CD" "cd / && pwd" "/" 0
run_test "Export variable" "export TEST=hello" "" 0
run_test "Echo exported variable" "export TEST=hello && echo \$TEST" "hello" 0
run_test "Unset variable" "export TEST=hello && unset TEST && echo \$TEST" "" 0
run_test "Env command" "env" "" 0  # Just check it runs
run_test "Exit with code" "exit 42" "" 42

# 3. REDIRECTIONS TESTS  
echo -e "${BLUE}=== 3. REDIRECTIONS ===${NC}"

# Create test files
echo "test content" > /tmp/test_input.txt

run_test "Output redirection" "echo hello > /tmp/test_output.txt && cat /tmp/test_output.txt" "hello" 0
run_test "Input redirection" "cat < /tmp/test_input.txt" "test content" 0
run_test "Append redirection" "echo world >> /tmp/test_output.txt && cat /tmp/test_output.txt" "hello world" 0

# 4. PIPES TESTS
echo -e "${BLUE}=== 4. PIPES ===${NC}"

run_test "Simple pipe" "echo hello | cat" "hello" 0
run_test "Multiple pipes" "echo hello | cat | cat" "hello" 0
run_test "Pipe with grep" "echo -e 'hello\nworld' | grep hello" "hello" 0

# 5. ENVIRONMENT VARIABLES TESTS
echo -e "${BLUE}=== 5. ENVIRONMENT VARIABLES ===${NC}"

run_test "HOME variable" "echo \$HOME" "$HOME" 0
run_test "PATH variable exists" "echo \$PATH" "" 0  # Just check it doesn't crash
run_test "Question mark variable" "false && echo \$?" "1" 0
run_test "Dollar dollar" "echo \$\$" "" 0  # Process ID

# 6. QUOTES TESTS
echo -e "${BLUE}=== 6. QUOTES ===${NC}"

run_test "Single quotes" "echo 'hello world'" "hello world" 0
run_test "Double quotes" "echo \"hello world\"" "hello world" 0
run_test "Single quotes with variables" "echo '\$USER'" "\$USER" 0
run_test "Double quotes with variables" "echo \"\$USER\"" "$USER" 0
run_test "Mixed quotes" "echo 'hello \"world\"'" "hello \"world\"" 0

# 7. HEREDOC TESTS
echo -e "${BLUE}=== 7. HEREDOC ===${NC}"

run_interactive_test "Simple heredoc" "
cat << EOF
hello
world
EOF
"

# 8. ERROR HANDLING TESTS
echo -e "${BLUE}=== 8. ERROR HANDLING ===${NC}"

run_test "Command not found" "nonexistentcommand" "command not found" 127
run_test "Syntax error - pipe" "echo hello |" "syntax error" 2
run_test "Syntax error - redirect" "echo hello >" "syntax error" 2

# 9. SIGNAL HANDLING TESTS
echo -e "${BLUE}=== 9. SIGNAL HANDLING ===${NC}"

# These are harder to test automatically, but we can check basic functionality
run_interactive_test "Ctrl-C handling" "
./minishell &
PID=\$!
sleep 1
kill -INT \$PID
wait \$PID 2>/dev/null
echo 'Signal test completed'
"

# 10. COMPLEX COMBINATIONS
echo -e "${BLUE}=== 10. COMPLEX COMBINATIONS ===${NC}"

run_test "Export and pipe" "export TEST=hello && echo \$TEST | cat" "hello" 0
run_test "CD and PWD with pipe" "cd / && pwd | cat" "/" 0
run_test "Multiple redirections" "echo hello > /tmp/test1.txt && cat /tmp/test1.txt > /tmp/test2.txt && cat /tmp/test2.txt" "hello" 0

# Cleanup
rm -f /tmp/test_*.txt /tmp/minishell_test.sh

# Print summary
echo -e "${BLUE}=== TEST SUMMARY ===${NC}"
echo -e "Total tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

if [[ $FAILED_TESTS -eq 0 ]]; then
    echo -e "${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed. ✗${NC}"
    exit 1
fi
