NAME	=	minishell

# -------  ADD -Werror ! -------

FLAG	= 	-Wall -Wextra -I ./includes -I ./libft/
FLAG 	+=	-g -ggdb  -fsanitize=address
SRCS	=	sources/ft_apply_redir.c \
			sources/ft_assign.c \
			sources/ft_break.c \
			sources/ft_btree.c \
			sources/ft_echo.c \
			sources/ft_pwd.c \
			sources/ft_cd.c \
			sources/ft_exit.c \
			sources/ft_utils.c \
			sources/ft_context.c \
			sources/ft_cross.c \
			sources/ft_dico.c \
			sources/ft_error.c \
			sources/ft_exec.c \
			sources/ft_expander.c \
			sources/ft_pipes.c \
			sources/ft_prove.c \
			sources/ft_quoting.c \
			sources/ft_redir.c \
			sources/ft_redir_utils.c \
			sources/ft_search_path.c \
			sources/ft_sow.c \
			sources/ft_export.c \
			sources/ft_unset.c \
			sources/ft_env.c \
			sources/minishell.c \
			sources/ft_here_doc_open.c \
			tests/btree_show.c \
			tests/dico_show.c \

CC		=	gcc

OBJS	=	$(SRCS:.c=.o)

all		:	$(NAME)

$(NAME)	:	$(OBJS) 
			make bonus -j 4 -C libft
			$(CC) $(FLAG) -o $(NAME) $(OBJS) libft/libft.a libreadline.a -lcurses

%.o		:	%.c
			$(CC) $(FLAG) -o $@ -c $<  

clean	:
			rm -f $(OBJS) libft/*.o

fclean	:	clean 
			rm -f $(NAME) libft/libft.a

re		:	fclean all
