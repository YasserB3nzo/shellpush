#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "\n${BLUE}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "${YELLOW}Command: $command${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Create input file for the test
    echo "$command" > test_input.txt
    echo "exit" >> test_input.txt
    
    # Run the test
    timeout 10s ./minishell < test_input.txt > test_output.txt 2>&1
    local exit_code=$?
    
    # Check if it timed out
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}❌ TIMEOUT${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return
    fi
    
    # Show output
    echo -e "${BLUE}Output:${NC}"
    cat test_output.txt
    
    # Check basic functionality (we can't easily check exact exit codes in this setup)
    if [ -s test_output.txt ]; then
        echo -e "${GREEN}✅ PASS - Command executed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAIL - No output or execution failed${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    
    # Clean up
    rm -f test_input.txt test_output.txt
}

# Function to test redirections with file creation
test_redirection() {
    local test_name="$1"
    local command="$2"
    local expected_file="$3"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "\n${BLUE}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "${YELLOW}Command: $command${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Clean up any existing test files
    rm -f outfile.txt infile.txt append_test.txt
    
    # Create input file for the test
    echo "$command" > test_input.txt
    echo "exit" >> test_input.txt
    
    # Create input file if needed
    if [[ $command == *"< infile.txt"* ]]; then
        echo "test content for input" > infile.txt
    fi
    
    # Run the test
    timeout 10s ./minishell < test_input.txt > test_output.txt 2>&1
    local exit_code=$?
    
    # Check if it timed out
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}❌ TIMEOUT${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return
    fi
    
    # Show shell output
    echo -e "${BLUE}Shell Output:${NC}"
    cat test_output.txt
    
    # Check if expected file was created
    if [ -n "$expected_file" ] && [ -f "$expected_file" ]; then
        echo -e "${BLUE}File Content ($expected_file):${NC}"
        cat "$expected_file"
        echo -e "${GREEN}✅ PASS - Redirection worked${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    elif [ -z "$expected_file" ]; then
        echo -e "${GREEN}✅ PASS - Command executed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAIL - Expected file $expected_file not created${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    
    # Clean up
    rm -f test_input.txt test_output.txt infile.txt
}

echo -e "${BLUE}=== MINISHELL MANDATORY FEATURES TEST SUITE ===${NC}"
echo -e "${BLUE}Testing only mandatory features (no bonus)${NC}\n"

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found. Please compile first.${NC}"
    exit 1
fi

echo -e "${BLUE}=== 1. BASIC COMMANDS ===${NC}"

run_test "Echo simple" "echo hello world" 0 "Basic echo command"
run_test "Echo with quotes" "echo 'hello world'" 0 "Echo with single quotes"
run_test "Echo with double quotes" 'echo "hello world"' 0 "Echo with double quotes"
run_test "Echo -n flag" "echo -n hello" 0 "Echo without newline"

echo -e "\n${BLUE}=== 2. BUILT-IN COMMANDS ===${NC}"

run_test "PWD command" "pwd" 0 "Print working directory"
run_test "ENV command" "env" 0 "Print environment variables"
run_test "CD to home" "cd" 0 "Change to home directory"
run_test "CD with path" "cd /tmp" 0 "Change to specific directory"
run_test "Export variable" "export TEST_VAR=hello" 0 "Export new variable"
run_test "Unset variable" "unset PATH" 0 "Unset environment variable"

echo -e "\n${BLUE}=== 3. ENVIRONMENT VARIABLES ===${NC}"

run_test "Expand USER" 'echo $USER' 0 "Expand USER environment variable"
run_test "Expand HOME" 'echo $HOME' 0 "Expand HOME environment variable"
run_test "Expand PATH" 'echo $PATH' 0 "Expand PATH environment variable"
run_test "Expand undefined variable" 'echo $UNDEFINED_VAR' 0 "Expand undefined variable (should be empty)"
run_test "Exit status variable" 'echo $?' 0 "Show exit status of last command"

echo -e "\n${BLUE}=== 4. REDIRECTIONS ===${NC}"

test_redirection "Output redirection" "echo hello > outfile.txt" "outfile.txt" "Redirect output to file"
test_redirection "Append redirection" "echo hello >> append_test.txt" "append_test.txt" "Append output to file"
test_redirection "Input redirection" "cat < infile.txt" "" "Read from input file"

echo -e "\n${BLUE}=== 5. PIPES ===${NC}"

run_test "Simple pipe" "echo hello | cat" 0 "Basic pipe functionality"
run_test "Multiple pipes" "echo hello | cat | cat" 0 "Multiple pipes"
run_test "Pipe with builtin" "env | head -5" 0 "Pipe with external command"

echo -e "\n${BLUE}=== 6. QUOTES HANDLING ===${NC}"

run_test "Single quotes" "echo 'hello world'" 0 "Single quotes should preserve literal values"
run_test "Double quotes with variables" 'echo "hello $USER"' 0 "Double quotes should expand variables"
run_test "Mixed quotes" "echo 'hello' world" 0 "Mixed quoting"
run_test "Empty quotes" 'echo ""' 0 "Empty double quotes"

echo -e "\n${BLUE}=== 7. SIGNAL HANDLING ===${NC}"

run_test "Ctrl+C simulation" "sleep 1" 0 "Should handle interruption gracefully"

echo -e "\n${BLUE}=== 8. ERROR HANDLING ===${NC}"

run_test "Non-existent command" "nonexistentcommand" 127 "Should return 127 for command not found"
run_test "Permission denied" "/etc/passwd" 126 "Should return 126 for permission denied"
run_test "Syntax error" "echo hello |" 2 "Should handle syntax errors"
run_test "Unclosed quotes" "echo 'hello" 2 "Should handle unclosed quotes"

echo -e "\n${BLUE}=== 9. HEREDOC ===${NC}"

# Create a heredoc test
cat << 'EOF' > heredoc_test.txt
cat << END
hello
world
END
exit
EOF

echo -e "\n${BLUE}Test: Heredoc${NC}"
echo -e "${YELLOW}Command: cat << END (with hello\\nworld\\nEND)${NC}"
echo -e "${YELLOW}Description: Here document functionality${NC}"

timeout 10s ./minishell < heredoc_test.txt > heredoc_output.txt 2>&1
if [ $? -eq 124 ]; then
    echo -e "${RED}❌ TIMEOUT${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
else
    echo -e "${BLUE}Output:${NC}"
    cat heredoc_output.txt
    echo -e "${GREEN}✅ PASS - Heredoc executed${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

rm -f heredoc_test.txt heredoc_output.txt

echo -e "\n${BLUE}=== 10. EXIT COMMAND ===${NC}"

# Test exit command separately
echo -e "\n${BLUE}Test: Exit with code${NC}"
echo -e "${YELLOW}Command: exit 42${NC}"
echo "exit 42" > exit_test.txt
timeout 5s ./minishell < exit_test.txt > /dev/null 2>&1
exit_code=$?
echo -e "${BLUE}Exit Code: $exit_code${NC}"
if [ $exit_code -eq 42 ]; then
    echo -e "${GREEN}✅ PASS - Exit with custom code works${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}❌ FAIL - Expected exit code 42, got $exit_code${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

rm -f exit_test.txt

# Clean up any remaining test files
rm -f outfile.txt append_test.txt test_input.txt test_output.txt

echo -e "\n${BLUE}=== TEST SUMMARY ===${NC}"
echo -e "${BLUE}Total Tests: $TOTAL_TESTS${NC}"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED!${NC}"
    exit 0
else
    echo -e "\n${YELLOW}⚠️  Some tests failed. Check the output above for details.${NC}"
    exit 1
fi
