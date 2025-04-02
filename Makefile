NAME      := minishell
CFLAGS     := -g -Wall -Wextra -Werror 
LIB	:= libft/libft.a

SRC	:=	parsing/redirections_utils.c minishell.c utils.c signals.c \
		parsing/command_utils.c parsing/token_utils.c parsing/parsing.c \
		exec/exec_shell.c exec/builtins/exit.c exec/builtins/cd.c \
		exec/builtins/cd_utils.c exec/builtins/env.c exec/builtins/export.c \
		exec/builtins/unset.c exec/builtins/pwd.c parsing/heredoc.c \
		parsing/command_free.c parsing/quote_utils.c parsing/quote_utils3.c \
		parsing/quotes.c exec/builtins/neosnitch.c exec/builtins/echo.c \
		exec/external_command.c exec/multiples_cmd.c exec/exec_utils.c \
		parsing/parsing_utils.c parsing/free_redirections.c \
		parsing/redirec_inout.c parsing/parse_input.c \
		utils/ft_arraydup.c utils/env_utils.c parsing/syntax_utils.c \
		exec/builtins/export_utils.c exec/exec_utils2.c exec/external_utils.c \
		exec/exec_utils3.c exec/multiples_utils.c parsing/parse_input2.c \
		parsing/quote_utils2.c exec/builtins/export_utils1.c exec/path.c\
		exec/exec_utils4.c exec/state.c exec/pipes.c parsing/token_utils2.c \
		parsing/parsing_utils2.c parsing/heredoc_utils.c parsing/heredoc_utils2.c \
		exec/external_command2.c exec/exec_shell2.c exec/path2.c \
		exec/external_utils2.c exec/multiples_cmd2.c exec/forking.c

OBJ      := $(SRC:.c=.o)

%.o:            %.c
		$(CC) $(CFLAGS) -c $< -o $@

$(NAME):        $(OBJ) $(LIB)
		cp $(LIB) $(NAME)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIB) -lreadline

$(LIB):
		$(MAKE) -C libft

all:	$(NAME)


clean:
		$(MAKE) clean -C libft
		rm -f $(OBJ)

fclean:         clean
		rm -f $(NAME)
		rm -f $(LIB)

re:             fclean all

.PHONY:         all clean fclean re