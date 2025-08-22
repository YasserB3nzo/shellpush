NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -ggdb3 -g3 -fsanitize=address -fno-omit-frame-pointer

SRCS = 	Libft/ft_split.c\
		Libft/ft_putstr_fd.c\
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
		minishell.c\
		src/parsing/parser.c\
		src/parsing/parse1.c\
		src/parsing/parse2.c\
		src/parsing/parse3.c\
		src/parsing/parse4.c\
		src/parsing/parse5.c\
		src/parsing/parse6.c\
		src/parsing/parse7.c\
		src/parsing/parse8.c\
		src/parsing/expander.c\
		src/parsing/heredoc.c\
		src/execution/exec.c\
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
		src/errors/shellerr.c\
		src/errors/syserr.c\
		src/utils/varproc.c\

OBJ = $(SRCS:.c=.o)

AR  = ar rcs

RM = rm -rf 

all : $(NAME)

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
