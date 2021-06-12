#include "minishell.h"

void	ft_export_var(t_var *var, char *arg, t_dico *dico)
{
	if (ft_strchr(arg, '='))
		ft_set_dico_value(var->key, var->value, GLOBAL, dico);
	else
	{
		free(var->key);
		var = ft_get_dico_var(arg, dico);
		if (var)
			var->scope = GLOBAL;
		else
			ft_set_dico_value(ft_strdup(arg), NULL, EXPORT, dico);
	}
}

int	ft_export(t_btree *node, t_dico *dico)
{
	char	**argv;
	t_var	*var;
	int		code_return;

	code_return = 0;
	argv = node->argv + 1;
	if (!(*argv))
		return (ft_show_envp(dico, 1, node->fd[1]));
	while (*argv)
	{
		var = ft_str_to_var(*argv, 1);
		if (var)
		{
			ft_export_var(var, *argv, dico);
			free(var);
		}
		else
		{
			ft_error((const char *[]){_strerror(EEXPORT), \
					"`", *argv, "': not a valid identifier\n", NULL}, FALSE);
			code_return = 1;
		}
		argv++;
	}
	return (code_return);
}
