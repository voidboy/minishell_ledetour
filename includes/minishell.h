#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <term.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <curses.h>
#include "libft.h"

# define CMD_FOUND_NX	126
# define CMD_NFOUND		127
# define ERROR			-1
# define SUCCESS		0
# define FAILURE		1
# ifndef ELAST
# define ELAST			0x1000
# endif
# define EPARSE			(ELAST + 1)
# define EMULTI			(ELAST + 2)
# define EEMPTY			(ELAST + 3)
# define EEXPORT		(ELAST + 4)
# define EUNSET			(ELAST + 5)
# define NBFCT			7	

/* >>> Lecture de la commande [TODO] */

/* Gestion erreur [TODO] */

/* >>> Parser */
typedef enum e_way { 
	IN,
	IN_IN,
	OUT,
	OUT_OUT,
}			t_way;

typedef enum e_type { 
	SEMICOLON,
	AND,
	OR,
	PIPE,
	CMD,
	NEWLINE,
}			t_type;

typedef enum e_scope { 
	LOCAL,
	GLOBAL,
	EXPORT,
}			t_scope;

typedef _Bool t_bool;

typedef enum e_side { 
	ROOT,
	LEFT,
	RIGHT,
}			t_side;

typedef struct s_context { 
	t_bool	inside_Squote;
	t_bool	inside_Dquote;
	t_bool	escape;
}				t_context;

typedef struct s_redir { 
	char			*filename;
	t_way			way;
}				t_redir;

typedef struct s_btree { 
	struct s_btree	*parent;
	struct s_btree	*left;
	struct s_btree	*right;
	int				id;
	t_type			type;
	t_side			side;
	t_list 			*redir;
	char			*cmd;
	char			**argv;
	int				fd[2];
	char			*buff;
	char			*delimiter;
}				t_btree;

typedef struct s_var { 
	char			*key;
	char			*value;
	t_scope			scope;
}				t_var;

typedef struct s_dico {
	t_list		*sets;
	char		**envp;
}				t_dico;

typedef struct s_index_var
{
	int	equal;
	int	key;
	int value;
}				t_index_var;

typedef struct s_minishell
{
	t_btree	*root;
	t_dico	*dico;
}				t_minishell;

/* >>> Gestion erreur */
int		ft_error(const char *msg[], t_bool is_fatal);
void	parse_error(t_btree *node, char last);
int		check_left(t_btree *node);
int		check_right(t_btree *node);
char	*_strerror(int _errno);

/* >>> Parsing */
t_way trim_redir(char *cmd, int *i);
t_bool	is_delimiter(char c);
t_bool	is_redirection(char c);

/* >>> Dico */
char *ft_get_dico_value(char *key, t_dico *dico);
void ft_show_dico(void *content);
int	ft_set_dico_value(char *key, char *value, t_scope scope, t_dico *dico);
int ft_set_dico(t_dico *dico, char **envp);
int	ft_new_dico_var(char *key, char *value, t_scope scope, t_dico *dico);

t_var *ft_str_to_var(char *str, int verify);
t_var *ft_get_dico_var(char *key, t_dico *dico);
int ft_show_envp(t_dico *dico, int declare, int fd);
int	ft_rm_dico_var(char *key, t_dico *dico);

/* construction de l'arbre d'execution */
t_btree *ft_sow(char *line);

/* verification syntaxique, gestion des redirections, si necessaire affiche l'erreur */
int ft_prove(t_btree *root);

/* her doc */
int ft_open_her_doc(t_btree *root);

/* parcours des commandes, infix */
int ft_cross(t_btree *root, t_dico *dico);

/* preparer les pipes */
void ft_pipes(t_btree *node);
void	ft_cleanup_fd(t_btree *node);

/* assignation */
int ft_assign(t_btree *node, t_dico *dico);

/* redirection */
int ft_redir(t_btree *node);
int ft_apply_redir(t_btree *node, t_dico *dico);

/* execution */
char *ft_search_path(char *exec, char *path);
int	ft_exec(t_btree *node, t_dico *dico);

/* >>> Utils */

/* sanitize */
char *ft_sanitize(char *str);

/* gnl */
int	get_next_line(int fd, char **line);

/* expension */
char *ft_expander(char *str, t_dico *dico);

/* quoting */
int	ft_quoting(char *str, int *i, char *newstr);

/* break */
char	**ft_break(char *str, char *charset);

/* >>> Built-in */

/* echo */
int ft_echo(t_btree *node, t_dico *dico);

/* cd */
int ft_cd(t_btree *node, t_dico *dico);

/* pwd */
int ft_pwd(t_btree *node, t_dico *dico);

/* export */
int ft_export(t_btree *node, t_dico *dico);

/* unset */
int ft_unset(t_btree *node, t_dico *dico);

/* env */
int ft_env(t_btree *node, t_dico *dico);

/* exit */
int ft_exit(t_btree *node, t_dico dico);

/* >>> btree library */
void btree_apply_infix(t_btree *root, void (*applyf)(void *));
void btree_free(t_btree *root);
int	btree_level_count(t_btree *root);
void btree_free(t_btree *root);
t_btree *rightest_node(t_btree *root);

/* >>> context */
void	init_context(t_context *context);
t_bool	is_context_free(t_context context);
t_bool	is_econtext_free(t_context context);
void	update_context(t_context *context, char current);
void	context_error(void);

/* >>> tests */
void btree_show(t_btree *root);
void ft_printstrs(char **strs, const char *prefix);

extern t_minishell g_minishell;

#endif
