#!/bin/bash
# Automated minishell mandatory tests

MINISHELL=./minishell

function test_case() {
    DESC="$1"
    CMD="$2"
    EXPECT="$3"
    echo -n "[TEST] $DESC ... "
    OUT=$(echo -e "$CMD" | $MINISHELL 2>&1)
    if [[ "$OUT" == *"$EXPECT"* ]]; then
        echo "OK"
    else
        echo "FAIL"
        echo "  Output: $OUT"
    fi
}

echo "--- MANDATORY TESTS ---"

test_case "Invalid command error" "foo\nexit" "command not found"
test_case "/bin/ls" "/bin/ls\nexit" "Makefile"
test_case "/bin/ls -laF" "/bin/ls -laF\nexit" "Makefile"
test_case "/bin/ls -l -a -F" "/bin/ls -l -a -F\nexit" "Makefile"

test_case "exit builtin" "exit" ""
test_case "echo with quotes" "echo 'It works'\nexit" "It works"
test_case "echo without quotes" "echo It works\nexit" "It works"
test_case "cd absolute path" "cd /tmp\n/bin/pwd\nexit" "/tmp"
test_case "cd relative path" "cd ..\n/bin/pwd\nexit" "/workspaces"
test_case "cd home" "cd\n/bin/pwd\nexit" "$HOME"
test_case "cd -" "cd /tmp\ncd -\n/bin/pwd\nexit" "$PWD"
test_case "cd tilde" "cd ~\n/bin/pwd\nexit" "$HOME"

test_case "env" "env\nexit" "PATH="
test_case "setenv FOO bar" "export FOO=bar\nenv\nexit" "FOO=bar"
test_case "echo $FOO" "export FOO=bar\necho $FOO\nexit" "bar"
test_case "/usr/bin/env with FOO" "export FOO=bar\n/usr/bin/env\nexit" "FOO=bar"
test_case "unsetenv FOO" "export FOO=bar\nunset FOO\nenv\nexit" "FOO=bar"
test_case "unsetenv FOO again" "unset FOO\nenv\nexit" "FOO="
test_case "/usr/bin/env without FOO" "unset FOO\n/usr/bin/env\nexit" "FOO="

test_case "unsetenv PATH and setenv PATH" "unset PATH\nexport PATH=/bin:/usr/bin\nls\nexit" "Makefile"
test_case "emacs" "emacs --version\nexit" "emacs"
test_case "unsetenv PATH and ls fail" "unset PATH\nls\nexit" "command not found"
test_case "unsetenv PATH and emacs fail" "unset PATH\nemacs\nexit" "command not found"
test_case "/bin/ls with unset PATH" "unset PATH\n/bin/ls\nexit" "Makefile"
test_case "/usr/bin/emacs with unset PATH" "unset PATH\n/usr/bin/emacs --version\nexit" "emacs"

test_case "empty command" "\nexit" "minishell$"
test_case "spaces only" "    \nexit" "minishell$"
test_case "spaces and tabs" " \t \t\nexit" "minishell$"
test_case "spaces/tabs around command" "   /bin/ls   -l   -A   \nexit" "Makefile"

echo "--- END OF TESTS ---"
