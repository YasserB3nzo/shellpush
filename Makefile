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
		src/parsing/parsing.c\
		src/parsing/parsing_utils1.c\
		src/parsing/parsing_utils2.c\
		src/parsing/parsing_utils3.c\
		src/parsing/parsing_utils4.c\
		src/parsing/parsing_utils5.c\
		src/parsing/parsing_utils6.c\
		src/parsing/parsing_utils7.c\
		src/parsing/parsing_utils8.c\
		src/parsing/expand_variable.c\
		src/parsing/open_heredoc.c\
		src/execution/executing.c\
		src/execution/handlingsignals.c\
		src/builtins/cd.c\
		src/builtins/echo.c\
		src/builtins/env.c\
		src/builtins/export.c\
		src/builtins/pwd.c\
		src/builtins/unset_exit.c\
		src/utils/lst.c\
		src/utils/tmp.c\
		src/utils/utils0.c\
		src/utils/utils1.c\
		src/utils/utils2.c\
		src/utils/utils3.c\
		src/utils/utils4.c\
		src/utils/utils5.c\
		src/utils/utils6.c\
		src/utils/utils7.c\
		src/utils/utils8.c\
		src/utils/utils9.c\
		src/errors/errors0.c\
		src/errors/errors1.c\
		src/utils/processvarexp.c\

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
