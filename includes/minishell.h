#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <term.h>
# include <dirent.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <ctype.h>
# include <termios.h>
# include <string.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include "libft.h"

# define PARSE_ERR		258
# define EXIT_ERR		255
# define CMD_FOUND_NX	126
# define CMD_NFOUND		127
# define SIG_TERM_NUM	128
# define EXIT_FORK		300
# define ERROR			-1
# define SUCCESS		0
# define FAILURE		1
# define EOFFSET		0x1000
# define EPARSE			0x1001
# define EMULTI			0x1002
# define EEMPTY			0x1003
# define EEXPORT		0x1004
# define EUNSET			0x1005
# define ECD			0x1006
# define EEXIT			0x1007
# define EECHO			0x1008
# define EPWD			0x1009
# define NBFCT			7	

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

typedef enum e_bool{
	FALSE,
	TRUE,
}			t_bool;

typedef struct s_counter{
	int	i;
	int	j;
	int	k;
}			t_counter;

typedef enum e_side {
	ROOT,
	LEFT,
	RIGHT,
}			t_side;

typedef enum e_stage{
	PARENT,
	HERE_OPEN,
	CHILD,
}			t_stage;

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
	t_list			*redir;
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
	int	value;
}				t_index_var;

typedef struct s_minishell
{
	t_btree	*root;
	t_dico	*dico;
}				t_minishell;

typedef const char	*t_strs[];

/* >>> Gestion erreur */
int		ft_error(const char *msg[], t_bool is_fatal);
void	parse_error(t_btree *node, char last);
int		check_left(t_btree *node);
int		check_right(t_btree *node);
char	*_strerror(int _errno);
int		ft_free_strs(char **strs);
void	ft_free_node(void *_node);
void	ft_free(t_btree *root, t_dico *dico);

/* >>> Parsing */
t_way	trim_redir(char *cmd, int *i);
t_bool	is_delimiter(char c);
t_bool	is_redirection(char c);

/* >>> Dico */
void	ft_escape_declare(char *str, int fd);
void	ft_show_var(t_var *var, int declare, int fd);
int		ft_show_envp(t_dico *dico, int declare, int fd);

t_var	*ft_get_dico_var(char *key, t_dico *dico);
int		ft_rm_dico_var(char *key, t_dico *dico);
int		ft_new_dico_var(char *key, char *value, t_scope scope, t_dico *dico);
int		ft_set_dico_value(char *key, char *value, t_scope scope, t_dico *dico);

char	*ft_get_dico_value(char *key, t_dico *dico);
t_var	*ft_str_to_var(char *str, int verify);
int		ft_sets_size_global(t_list *lst);
int		ft_set_envp(t_dico *dico);
int		ft_set_dico(t_dico *dico, char **envp);

/* >>> Core */

/* tree building  */
t_btree	*ft_sow(char *line);
t_type	ft_split_op(char *line, char **s_op, char **s_left, char **s_right);
t_btree	*btree_new_node(char *cmd, t_btree *parent, t_side side, t_type op);
t_btree	*ft_wrap_sow(char *line, t_btree *parent, t_side side);

char	*ft_cmdtrim(char *s);
int		ft_find_last(char *hay, char *need);
int		ft_op_is_finded(t_type *op, int *index);

/* syntax verification */
int		ft_prove(t_btree *root);

/* her doc */
int		ft_here_doc_open(t_btree *root);
int		ft_readoneline(t_btree *node);
void	ft_here_doc_read(void *_node);

/* commands lookup INFIX */
int		ft_cross(t_btree *root, t_dico *dico);

/* pipes */
void	ft_pipes(t_btree *node);
void	ft_cleanup_fd(t_btree *node);

/* assignation */
int		ft_assign(t_btree *node, t_dico *dico);
void	ft_set_var(t_btree *node, t_var *var, t_index_var *ivar);
int		ft_extract_assign(t_btree *node, t_list *lstassign, t_var *vars);
int		ft_apply_assign(t_var *vars, t_dico *dico, int nbvars);
void	ft_create_argv(t_btree *node, t_dico *dico);

void	ft_free_lstassign(void *ivar);
int		ft_free_vars(t_var *var, int len, int r);
int		ft_get_varkey(char *str, int i);
int		ft_get_varvalue(char *str, int i);
int		ft_goto_assign(char *str, int *i);

void	ft_find_assign(char *str, t_list **lstassign, int index);
int		ft_len_extact_assign(char *str, t_list *lstassign);
char	*ft_create_newcmd(t_btree *node, t_list *lstassign, t_var *vars);

/* redirection */
int		ft_redir(t_btree *node);
int		ft_apply_redir(t_btree *node, t_dico *dico);

/* execution */
char	*ft_search_path(char *exec, char *path);
int		ft_exec(t_btree *node, t_dico *dico);

/* >>> Utils */
void	echo_control_seq(t_bool c);
char	*add_linefeed(char *str);
int		ft_tolong(const char *str, long long *nb);

void	rl_replace_line (const char *text, int clear_undo);
void	sig_hand_child(int n);
void	sig_hand_parent(int n);
void	sig_hand_here(int n);
void	sig_apply(t_stage stage);

/* sanitize */
char	*ft_sanitize(char *str);
int		ft_len_sanitize(char *str);

/* expension */
char	*ft_expander(char *str, t_dico *dico);

/* quoting */
int	ft_dbl_quoting(char *str, int *i, char *newstr);
int	ft_simple_quoting(char *str, int *i, char *newstr);
int	ft_backslash(char *str, int *i, char *newstr);
int	ft_quoting(char *str, int *i, char *newstr);

/* break */
char	**ft_break(char *str, char *charset);
char	*ft_strstr(char *str, char *to_find);
char	**ft_add_str(char **tab_str, char *str, int size_tab);
char	*ft_create_str(char *start, char *end);

/* >>> Built-in */

/* echo */
int		ft_echo(t_btree *node, t_dico *dico);

/* cd */
int		ft_cd(t_btree *node, t_dico *dico);

/* pwd */
int		ft_pwd(t_btree *node, t_dico *dico);

/* export */
int		ft_export(t_btree *node, t_dico *dico);
t_var	*ft_str_to_varplus(char *str, int *plus);
void	ft_join_var(t_var *var, t_dico *dico);
void	ft_export_var(t_var *var, char *arg, t_dico *dico, int plus);
void	ft_print_export_error(char *str, t_var *var, int *code_return);

/* unset */
int		ft_unset(t_btree *node, t_dico *dico);
int		ft_isvalid_id(char *str);

/* env */
int		ft_env(t_btree *node, t_dico *dico);

/* exit */
int		ft_exit(t_btree *node, t_dico *dico);

/* >>> btree library */
void	btree_apply_infix(t_btree *root, void (*applyf)(void *));
void	btree_apply_suffix(t_btree *root, void (*applyf)(void *));
void	btree_free(t_btree *root);
int		btree_level_count(t_btree *root);
void	btree_free(t_btree *root);
t_btree	*rightest_node(t_btree *root);
t_btree	*root_node(t_btree *node);

/* >>> context */
void	init_context(t_context *context);
t_bool	is_context_free(t_context context);
t_bool	is_econtext_free(t_context context);
void	update_context(t_context *context, char current);
void	context_error(void);

/* >>> tests */
void	btree_apply_infix(t_btree *root, void (*applyf)(void *));
void	btree_apply_suffix(t_btree *root, void (*applyf)(void *));
int		ft_max(int i, int j);
int		btree_level_count(t_btree *root);
t_btree	*rightest_node(t_btree *root);

#endif
