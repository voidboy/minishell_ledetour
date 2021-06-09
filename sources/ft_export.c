#include "minishell.h"

int ft_export(t_btree *node, t_dico *dico)
{
	char **argv;
	t_var	*var;
	int		code_return;

	code_return = 0;
	printf("#### export\n");
	argv = node->argv + 1;
	if (!(*argv))
		return (ft_show_envp(dico, 1, node->fd[1]));
	while (*argv)
	{
		var = ft_str_to_var(*argv, 1);
		if (var)
		{
			if (ft_strchr(*argv, '='))
			{
				ft_set_dico_value(var->key, var->value, GLOBAL, dico);
				free(var);
			}
			else
			{
				free(var->key);
				free(var);
				var = ft_get_dico_var(*argv, dico);
				if (var)
					var->scope = GLOBAL;
				else
					ft_set_dico_value(*argv, NULL, EXPORT, dico);
			}
		}else
		{
			ft_error(EEXPORT, \
					(const char *[]){"`",*argv ,"': not a valid identifier\n", NULL});
			code_return = 1;
		}
		argv++;
	}
	return (code_return);
}
