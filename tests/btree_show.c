#include "minishell.h" 

# define LEVMAX 200
# define WMAX 255

int _print_t(t_btree *tree, int is_left, int offset, int depth, char s[LEVMAX][WMAX])
{
    char b[LEVMAX];
    int width = 10;
	int r, l;
	char *w = " ";

    if (!tree) return 0;
	l = ft_strlen(tree->cmd);
	r = (width - 2 - l) / 2; 
    sprintf(b, "%*s{%s}%*s", r,w,tree->cmd,(width -2) - (r+l),w);

    int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '_';

        s[2 * depth - 1][offset + left + width + right + width/2] = '|';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '_';

        s[2 * depth - 1][offset - width/2 - 1] = '|';
    }

    return left + width + right;
}

void ft_printstrs(char **strs, const char *prefix)
{
	printf("{%s} : ", prefix);
	while (strs && *strs)
	{
		printf(" [%s] ", *strs);
		strs++;
	}
	printf("\n");
}

void ft_displredr(void *r)
{
	t_redir *redir = r;

	printf("{FILENAME} : %s\n", redir->filename);
	printf("{WAY} : %s\n", redir->way == IN ? "in" : redir->way == OUT ? "out" : "out_out");
}


void print_node(void *item)
{
	t_btree *node;
	int		idp;
	
	node = (t_btree *)item;
	idp = -1;
	if (node->parent)
		idp = node->parent->id;
	printf("ID %03d => cmd : [%s]", node->id, node->cmd);
	printf(" | type : %d", node->type);
	printf(" | side : %d", node->side);
	printf(" | idp  : %d\n", idp);
	ft_printstrs(node->argv, "ARGV");
	ft_lstiter(node->redir, ft_displredr);
}

void btree_show(t_btree *root)
{
    char s[LEVMAX][WMAX];
	int level;
	int width;

	width = 200;
	level = btree_level_count(root) * 2;
    for (int i = 0; i <= level; i++)
        sprintf(s[i], "%*s", width," ");

    _print_t(root, 0, 0, 0, s);

    for (int i = 0; i <= level; i++)
        printf("%s\n", s[i]);

	btree_apply_infix(root, print_node);
}
