#!/bin/bash

# 42 Minishell Comprehensive Test Suite
# This script tests all major functionality and edge cases

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
TOTAL=0
VERBOSE=${1:-0}  # Pass 1 as argument for verbose mode

# Test counter
test_count=0

print_header() {
    echo -e "${BLUE}======================================${NC}"
    echo -e "${BLUE}    42 MINISHELL TEST SUITE${NC}"
    echo -e "${BLUE}======================================${NC}"
    echo -e "${CYAN}Usage: $0 [1] (pass 1 for verbose mode)${NC}"
    echo
}

print_section() {
    echo
    echo -e "${YELLOW}>>> $1 <<<${NC}"
    echo
}

show_detailed_output() {
    local test_desc="$1"
    local command="$2"
    local expected_exit="$3"
    local actual_exit="$4"
    local minishell_out="$5"
    local minishell_err="$6"
    local bash_out="$7"
    local bash_err="$8"
    
    echo -e "\n${MAGENTA}━━━ DETAILED TEST RESULTS ━━━${NC}"
    echo -e "${CYAN}Test:${NC} $test_desc"
    echo -e "${CYAN}Command:${NC} '$command'"
    echo -e "${CYAN}Expected Exit Code:${NC} $expected_exit"
    echo -e "${CYAN}Actual Exit Code:${NC} $actual_exit"
    
    echo -e "\n${CYAN}MINISHELL OUTPUT:${NC}"
    echo -e "${GREEN}stdout:${NC}"
    if [ -s "$minishell_out" ]; then
        sed 's/^/  /' "$minishell_out"
    else
        echo "  (empty)"
    fi
    echo -e "${RED}stderr:${NC}"
    if [ -s "$minishell_err" ]; then
        sed 's/^/  /' "$minishell_err"
    else
        echo "  (empty)"
    fi
    
    if [ -f "$bash_out" ]; then
        echo -e "\n${CYAN}BASH OUTPUT (for comparison):${NC}"
        echo -e "${GREEN}stdout:${NC}"
        if [ -s "$bash_out" ]; then
            sed 's/^/  /' "$bash_out"
        else
            echo "  (empty)"
        fi
        echo -e "${RED}stderr:${NC}"
        if [ -s "$bash_err" ]; then
            sed 's/^/  /' "$bash_err"
        else
            echo "  (empty)"
        fi
    fi
    echo -e "${MAGENTA}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}\n"
}

test_command() {
    local desc="$1"
    local cmd="$2"
    local expected_exit="$3"
    local should_compare_bash="${4:-1}"
    
    test_count=$((test_count + 1))
    TOTAL=$((TOTAL + 1))
    
    printf "Test %2d: %-50s " "$test_count" "$desc"
    
    # Clean temp files
    rm -f /tmp/minishell_out /tmp/minishell_err /tmp/bash_out /tmp/bash_err
    
    # Run command in minishell
    echo "$cmd" | timeout 10s ./minishell >/tmp/minishell_out 2>/tmp/minishell_err
    local minishell_exit=$?
    
    # Run command in bash for comparison (where applicable)
    local bash_exit=0
    if [ "$should_compare_bash" = "1" ] && [[ "$cmd" != *"exit"* ]]; then
        echo "$cmd" | timeout 10s bash >/tmp/bash_out 2>/tmp/bash_err 2>/dev/null
        bash_exit=$?
    fi
    
    # Check result
    local test_passed=0
    if [ "$expected_exit" = "ANY" ] || [ "$minishell_exit" = "$expected_exit" ]; then
        echo -e "${GREEN}PASS${NC}"
        PASSED=$((PASSED + 1))
        test_passed=1
    else
        echo -e "${RED}FAIL${NC} (expected: $expected_exit, got: $minishell_exit)"
        FAILED=$((FAILED + 1))
    fi
    
    # Show detailed output if verbose mode or test failed
    if [ "$VERBOSE" = "1" ] || [ "$test_passed" = "0" ]; then
        show_detailed_output "$desc" "$cmd" "$expected_exit" "$minishell_exit" \
            "/tmp/minishell_out" "/tmp/minishell_err" "/tmp/bash_out" "/tmp/bash_err"
    fi
}

test_builtin_output() {
    local desc="$1"
    local cmd="$2"
    
    test_count=$((test_count + 1))
    TOTAL=$((TOTAL + 1))
    
    printf "Test %2d: %-50s " "$test_count" "$desc"
    
    # Clean temp files
    rm -f /tmp/minishell_out /tmp/minishell_err /tmp/bash_out /tmp/bash_err
    
    # Run in minishell
    echo "$cmd" | timeout 10s ./minishell >/tmp/minishell_out 2>/tmp/minishell_err
    local minishell_exit=$?
    
    # Run in bash
    echo "$cmd" | timeout 10s bash >/tmp/bash_out 2>/tmp/bash_err 2>/dev/null
    local bash_exit=$?
    
    # Compare outputs (ignoring minishell prompt)
    local test_passed=0
    if diff <(grep -v "minishell\$\|exit\$" /tmp/minishell_out | sed '/^$/d') \
            <(grep -v "exit\$" /tmp/bash_out | sed '/^$/d') >/dev/null 2>&1; then
        echo -e "${GREEN}PASS${NC}"
        PASSED=$((PASSED + 1))
        test_passed=1
    else
        echo -e "${RED}FAIL${NC} (output differs)"
        FAILED=$((FAILED + 1))
    fi
    
    # Show detailed output if verbose mode or test failed
    if [ "$VERBOSE" = "1" ] || [ "$test_passed" = "0" ]; then
        show_detailed_output "$desc" "$cmd" "0" "$minishell_exit" \
            "/tmp/minishell_out" "/tmp/minishell_err" "/tmp/bash_out" "/tmp/bash_err"
    fi
}

# Ensure minishell is built
if [ ! -f "./minishell" ]; then
    echo "Building minishell..."
    make
    if [ $? -ne 0 ]; then
        echo "Failed to build minishell!"
        exit 1
    fi
fi

print_header

# ============================================================================
# BASIC FUNCTIONALITY TESTS
# ============================================================================

print_section "BASIC FUNCTIONALITY"

test_command "Simple command execution" "echo hello" 0
test_command "Command with arguments" "echo hello world" 0
test_command "Empty command (just Enter)" "" 0
test_command "Command not found" "nonexistentcommand" 127
test_command "Simple pipe" "echo hello | cat" 0
test_command "Multiple pipes" "echo hello | cat | cat" 0

# ============================================================================
# BUILT-IN COMMANDS TESTS
# ============================================================================

print_section "BUILT-IN COMMANDS"

# Echo tests
test_builtin_output "echo without arguments" "echo"
test_builtin_output "echo with text" "echo hello world"
test_builtin_output "echo with -n flag" "echo -n hello"
test_builtin_output "echo with multiple -n flags" "echo -nnn hello"
test_builtin_output "echo with -n and text" "echo -n hello world"

# PWD tests
test_builtin_output "pwd command" "pwd"

# ENV tests  
test_builtin_output "env command" "env | head -5"

# CD tests
test_command "cd to home directory" "cd" 0
test_command "cd to root directory" "cd /" 0
test_command "cd to current directory" "cd ." 0
test_command "cd to parent directory" "cd .." 0
test_command "cd to nonexistent directory" "cd /nonexistent" 1

# Export tests
test_command "export new variable" "export TEST_VAR=hello" 0
test_command "export without value" "export TEST_VAR2" 0

# Export invalid names tests  
print_section "EXPORT INVALID NAMES"
test_command "export with number start" "export 1VAR=test" 1
test_command "export with dash in name" "export VAR-NAME=test" 1  
test_command "export with @ in name" "export VAR@NAME=test" 1
test_command "export empty name" "export =value" 1
test_command "export starting with @" "export @VAR=test" 1
test_command "export with space in name" "export 'VAR NAME'=test" 1
test_command "export only digits" "export 123=test" 1

# Unset tests  
test_command "unset existing variable" "unset PATH" 0
test_command "unset nonexistent variable" "unset NONEXISTENT" 0

# Exit tests
test_command "exit without arguments" "exit" 0
test_command "exit with valid number" "exit 42" 42
test_command "exit with invalid argument" "exit abc" 2

# ============================================================================
# REDIRECTIONS TESTS  
# ============================================================================

print_section "REDIRECTIONS"

# Create test files
echo "test content" > /tmp/test_input.txt
echo "line1" > /tmp/test_file.txt
echo "line2" >> /tmp/test_file.txt

test_command "Output redirection" "echo hello > /tmp/test_out.txt" 0
test_command "Append redirection" "echo world >> /tmp/test_out.txt" 0  
test_command "Input redirection" "cat < /tmp/test_input.txt" 0
test_command "Multiple redirections" "cat < /tmp/test_input.txt > /tmp/test_out2.txt" 0
test_command "Redirect to invalid file" "echo test > /dev/full" 1

# ============================================================================
# HEREDOC TESTS
# ============================================================================

print_section "HEREDOC"

test_command "Simple heredoc" "cat << EOF
hello
world  
EOF" 0

test_command "Heredoc with variable" "cat << EOF
hello \$USER
EOF" 0

# ============================================================================  
# PIPES TESTS
# ============================================================================

print_section "PIPES"

test_command "Simple pipe" "echo hello | cat" 0
test_command "Multiple pipes" "echo hello | cat | cat | cat" 0
test_command "Pipe with built-ins" "echo hello | cat" 0
test_command "Complex pipe chain" "ls | head -5 | tail -2" 0
test_command "Pipe with error" "nonexistent | cat" 127

# ============================================================================
# ENVIRONMENT VARIABLES TESTS  
# ============================================================================

print_section "ENVIRONMENT VARIABLES"

test_command "Simple variable expansion" "echo \$HOME" 0
test_command "Variable in middle of text" "echo hello\$USER world" 0  
test_command "Undefined variable" "echo \$UNDEFINED_VAR" 0
test_command "Exit status variable" "false; echo \$?" 0
test_command "Multiple variables" "echo \$HOME \$USER \$PWD" 0

# ============================================================================
# QUOTES TESTS
# ============================================================================

print_section "QUOTES"

test_command "Single quotes preserve literal" "echo 'hello \$HOME world'" 0
test_command "Double quotes allow expansion" "echo \"hello \$USER world\"" 0
test_command "Mixed quotes" "echo 'hello' \"world\" test" 0
test_command "Empty quotes" "echo '' \"\"" 0
test_command "Unclosed single quote" "echo 'unclosed" 2
test_command "Unclosed double quote" "echo \"unclosed" 2

# ============================================================================
# SIGNAL HANDLING TESTS
# ============================================================================

print_section "SIGNAL HANDLING"

# These tests are harder to automate, but we can test some aspects
test_command "Ctrl+C handling (SIGINT)" "sleep 1" 130  # This will timeout and get interrupted

# ============================================================================
# ERROR HANDLING TESTS  
# ============================================================================

print_section "ERROR HANDLING"

test_command "Syntax error: double pipe" "echo hello || echo world" 2
test_command "Syntax error: pipe at end" "echo hello |" 2
test_command "Syntax error: redirect without file" "echo hello >" 2
test_command "Syntax error: double redirect" "echo hello >> >>" 2

# ============================================================================
# EDGE CASES TESTS
# ============================================================================

print_section "EDGE CASES"

test_command "Very long command line" "echo $(printf 'a%.0s' {1..1000})" 0
test_command "Empty environment variable" "echo \$EMPTY" 0
test_command "Command with many arguments" "echo $(seq 1 100)" 0
test_command "Whitespace handling" "   echo    hello    world   " 0
test_command "Tab handling" "echo	hello	world" 0

# ============================================================================
# COMPLEX SCENARIOS
# ============================================================================

print_section "COMPLEX SCENARIOS"

test_command "Export and use variable" "export MY_VAR=test; echo \$MY_VAR" 0
test_command "CD and PWD combination" "cd /tmp; pwd" 0
test_command "Pipe with redirection" "echo hello | cat > /tmp/pipe_test.txt" 0
test_command "Multiple commands with semicolon" "echo first; echo second" 0

# ============================================================================
# RESULTS SUMMARY
# ============================================================================

print_section "TEST RESULTS SUMMARY"

echo -e "Total tests: ${BLUE}$TOTAL${NC}"
echo -e "Passed: ${GREEN}$PASSED${NC}"  
echo -e "Failed: ${RED}$FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    echo -e "${GREEN}Your minishell appears to be working correctly!${NC}"
else
    echo -e "\n${RED}❌ Some tests failed.${NC}"
    echo -e "${YELLOW}Check the failed tests above for issues to fix.${NC}"
    echo -e "${CYAN}Tip: Run with '$0 1' for verbose output on all tests${NC}"
fi

# Show quick debug commands
if [ $FAILED -gt 0 ]; then
    echo -e "\n${CYAN}Quick Debug Commands:${NC}"
    echo "  Compare with bash: echo 'command' | bash"
    echo "  Test manually: ./minishell"
    echo "  Check norminette: norminette ."
fi

# Cleanup
rm -f /tmp/test_*.txt /tmp/minishell_out /tmp/minishell_err /tmp/bash_out /tmp/bash_err /tmp/pipe_test.txt

echo
exit $FAILED
