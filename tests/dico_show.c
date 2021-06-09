#include "minishell.h"

void ft_show_dico(void *content)
{
	t_var *var;

	var = content;
	printf("DICO [ key : %s value : %s scope : %s]\n", var->key, var->value, var->scope == LOCAL ? "LOCAL" : "GLOBAL");
}
