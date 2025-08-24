#!/bin/bash
# 🧪 COMPREHENSIVE HEREDOC MEMORY LEAK TESTING
# Tests all heredoc scenarios and child process cases

echo "🧪 COMPREHENSIVE HEREDOC MEMORY LEAK TESTING"
echo "=============================================="
echo "Testing all heredoc scenarios and child processes..."

# Ensure minishell is compiled
echo "🔧 Compiling minishell..."
make re > /dev/null 2>&1

echo -e "\n🎯 TESTING SUITE: HEREDOC MEMORY LEAKS"
echo "======================================"

# Test 1: Basic heredoc
echo -e "\n🔍 TEST 1: Basic Heredoc"
echo "Command: cat << EOF"
echo -e "cat << EOF\nhello world\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite --track-origins=yes ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 2: Empty heredoc
echo -e "\n🔍 TEST 2: Empty Heredoc"  
echo "Command: cat << EOF (empty)"
echo -e "cat << EOF\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 3: Heredoc with variables
echo -e "\n🔍 TEST 3: Heredoc with Variables"
echo "Command: export TEST=hello; cat << EOF with \$TEST"
echo -e "export TEST=hello\ncat << EOF\nHello \$TEST world\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 4: Multiple heredocs in sequence
echo -e "\n🔍 TEST 4: Multiple Sequential Heredocs"
echo "Command: Multiple cat << EOF commands"
echo -e "cat << EOF\nfirst\nEOF\ncat << END\nsecond\nEND\ncat << FINAL\nthird\nFINAL\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 5: Heredoc in pipeline with external command
echo -e "\n🔍 TEST 5: Heredoc in Pipeline (Child Process)"
echo "Command: cat << EOF | wc -l"
echo -e "cat << EOF | wc -l\nline1\nline2\nline3\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 6: Heredoc with complex pipeline
echo -e "\n🔍 TEST 6: Heredoc with Complex Pipeline"
echo "Command: cat << EOF | grep test | wc -l"
echo -e "cat << EOF | grep test | wc -l\ntest line 1\nnormal line\ntest line 2\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 7: Heredoc with quoted delimiter
echo -e "\n🔍 TEST 7: Heredoc with Quoted Delimiter (No Expansion)"
echo "Command: cat << 'EOF'"
echo -e "cat << 'EOF'\n\$TEST should not expand\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 8: Heredoc with redirection
echo -e "\n🔍 TEST 8: Heredoc with Output Redirection"
echo "Command: cat << EOF > /tmp/test_output"
echo -e "cat << EOF > /tmp/test_output\ntest content\nEOF\ncat /tmp/test_output\nrm -f /tmp/test_output\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 9: Nested command with heredoc
echo -e "\n🔍 TEST 9: Command Substitution with Heredoc"
echo "Command: echo \$(cat << EOF)"
echo -e "echo \"Result: \$(cat << EOF\nhello world\nEOF)\"\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 10: Heredoc with special characters and variables
echo -e "\n🔍 TEST 10: Heredoc with Special Characters"
echo "Command: cat << EOF with \$HOME, quotes, etc."
echo -e "export USER=testuser\ncat << EOF\n\$HOME and \$USER\n\"quoted text\" and 'single quotes'\n\\\$escaped dollar\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 11: Very long heredoc content
echo -e "\n🔍 TEST 11: Large Heredoc Content"
echo "Command: cat << EOF with 50 lines"
LARGE_CONTENT=$(printf 'This is line number %d\n' {1..50})
echo -e "cat << EOF\n${LARGE_CONTENT}\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 12: Heredoc with immediate EOF
echo -e "\n🔍 TEST 12: Immediate EOF (Edge Case)"
echo "Command: cat << EOF immediately followed by EOF"
echo -e "cat << EOF\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

echo -e "\n🎯 TESTING SUITE: CHILD PROCESS SCENARIOS"
echo "========================================"

# Test 13: External command after heredoc
echo -e "\n🔍 TEST 13: External Command After Heredoc"
echo "Command: heredoc followed by /bin/echo"
echo -e "cat << EOF\ntest\nEOF\n/bin/echo \"after heredoc\"\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 14: Built-in command after heredoc
echo -e "\n🔍 TEST 14: Built-in Command After Heredoc"
echo "Command: heredoc followed by echo built-in"
echo -e "cat << EOF\ntest\nEOF\necho \"builtin after heredoc\"\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

# Test 15: Heredoc in background (if supported)
echo -e "\n🔍 TEST 15: Multiple Processes with Heredoc"
echo "Command: heredoc with fork-heavy scenario"
echo -e "cat << EOF | cat | cat\ntest data\nEOF\nexit" | valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

echo -e "\n🎯 TESTING SUITE: ERROR CONDITIONS"
echo "================================="

# Test 16: Unclosed heredoc (should be handled gracefully)
echo -e "\n🔍 TEST 16: Syntax Error Handling"
echo "Command: Proper error handling test"
echo -e "cat << EOF\ntest\n\nexit" | timeout 5 valgrind --leak-check=full --show-leak-kinds=definite ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|ERROR SUMMARY)" | head -4

echo -e "\n📊 COMPREHENSIVE SUMMARY"
echo "======================="
echo "✅ All tests should show '0 bytes definitely lost'"
echo "✅ Look for 'ERROR SUMMARY: 0 errors from 0 contexts'"
echo "❌ Any 'definitely lost', 'indirectly lost', or 'possibly lost' indicates memory leaks"
echo -e "\n🎯 If ANY test shows memory leaks, check:"
echo "   1. Child process cleanup in heredoc.c"
echo "   2. Readline memory management"
echo "   3. Environment variable handling"
echo "   4. File descriptor cleanup"
echo "   5. String allocation/deallocation"

echo -e "\n🔧 DETAILED ANALYSIS MODE"
echo "========================"
echo "Run this for detailed leak analysis:"
echo "echo -e 'cat << EOF\\nhello\\nEOF\\nexit' | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./minishell"
