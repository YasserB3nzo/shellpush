NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -ggdb3 -g3 -fsanitize=address -fno-omit-frame-pointer
VALGRIND_FLAGS = -Wall -Wextra -Werror -g

SRCS = 	Libft/ft_split.c\
		Libft/ft_putstr_fd.c\
		Libft/ft_putendl_fd.c\
		Libft/ft_strjoin.c\
		Libft/ft_strdup.c\
		Libft/ft_putchar_fd.c\
		Libft/ft_strnstr.c\
		Libft/ft_strncmp.c\
		Libft/ft_isalpha.c\
		Libft/ft_strlen.c\
		Libft/ft_lstsize_bonus.c\
		Libft/ft_strcmp.c\
		Libft/ft_isdigit.c\
		Libft/ft_isalnum.c\
		Libft/ft_strlcpy.c\
		Libft/ft_strchr.c\
		Libft/ft_itoa.c\
		Libft/ft_atoi.c\
		Libft/ft_substr.c\
		minishell.c\
		src/parsing/parser.c\
		src/parsing/parse1.c\
		src/parsing/parse1_tokens.c\
		src/parsing/parse1_errors.c\
		src/parsing/parse2.c\
		src/parsing/parse3.c\
		src/parsing/parse4.c\
		src/parsing/parse5.c\
		src/parsing/parse6.c\
		src/parsing/parse7.c\
		src/parsing/parse8.c\
		src/parsing/expander.c\
		src/parsing/heredoc.c\
		src/parsing/heredoc_utils.c\
		src/parsing/heredoc_helpers.c\
		src/parsing/heredoc_fork.c\
		src/execution/exec.c\
		src/execution/exec_utils.c\
		src/execution/signals.c\
		src/builtins/cd.c\
		src/builtins/echo.c\
		src/builtins/env.c\
		src/builtins/export.c\
		src/builtins/pwd.c\
		src/builtins/exit.c\
		src/utils/list.c\
		src/utils/temp.c\
		src/utils/util1.c\
		src/utils/util2.c\
		src/utils/util3.c\
		src/utils/util4.c\
		src/utils/util5.c\
		src/utils/util6.c\
		src/utils/util7.c\
		src/utils/util8.c\
		src/utils/util9.c\
		src/utils/util10.c\
		src/utils/util11.c\
		src/utils/util12.c\
		src/utils/util13.c\
		src/utils/util14.c\
		src/utils/util15.c\
		src/utils/util16.c\
		src/utils/util17.c\
		src/utils/env_utils.c\
		src/errors/shellerr.c\
		src/errors/shellerr_utils.c\
		src/errors/syserr.c\
		src/errors/addional.c\
		src/utils/varproc.c\

OBJ = $(SRCS:.c=.o)

AR  = ar rcs

RM = rm -rf 

all : $(NAME)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: re

valgrind: CFLAGS = $(VALGRIND_FLAGS)
valgrind: re

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o minishell -lreadline


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
		@$(RM) $(OBJ)

fclean :
		@$(RM) $(OBJ) $(NAME)

re : fclean all

.SECONDARY : ${OBJ}
