#include "minishell.h"

t_var	*ft_str_to_varplus(char *str, int *plus)
{
	t_var	*var;
	int		i;

	i = -1;
	var = NULL;
	while (str[++i] && str[i] != '+')
		;
	if (str[i] && str[i] == '+' && str[i + 1] == '=')
	{
		var = malloc(sizeof(t_var));
		if (!var)
			ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
		var->key = ft_substr(str, 0, i);
		var->value = ft_substr(str, i + 2, ft_strlen(str) - i - 1);
		var->scope = GLOBAL;
		*plus = 1;
	}
	return (var);
}

void	ft_join_var(t_var *var, t_dico *dico)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_get_dico_value(var->key, dico);
	tmp2 = var->value;
	var->value = ft_strjoin(tmp1, tmp2);
	free(tmp1);
	free(tmp2);
	ft_set_dico_value(var->key, var->value, GLOBAL, dico);
}

void	ft_export_var(t_var *var, char *arg, t_dico *dico, int plus)
{
	if (ft_strchr(arg, '='))
	{
		if (!plus)
			ft_set_dico_value(var->key, var->value, GLOBAL, dico);
		else
			ft_join_var(var, dico);
	}
	else
	{
		free(var->key);
		var = ft_get_dico_var(arg, dico);
		if (var)
			var->scope = GLOBAL;
		else
			ft_set_dico_value(ft_strdup(arg), NULL, EXPORT, dico);
	}
	free(var);
}

void	ft_print_export_error(char *str, t_var *var, int *code_return)
{
	ft_error((const char *[]){_strerror(EEXPORT), \
					"`", str, "': not a valid identifier\n", NULL}, FALSE);
	*code_return = 1;
	if (var)
	{
		free(var->key);
		free(var->value);
	}
	free(var);
}

int	ft_export(t_btree *node, t_dico *dico)
{
	char	**argv;
	t_var	*var;
	int		code_return;
	int		plus;

	plus = 0;
	code_return = 0;
	argv = node->argv + 1;
	if (!(*argv))
		return (ft_show_envp(dico, 1, node->fd[1]));
	while (*argv)
	{
		var = NULL;
		if (**argv)
			var = ft_str_to_var(*argv, 1);
		if (**argv && !var)
			var = ft_str_to_varplus(*argv, &plus);
		if (var && var->key)
			ft_export_var(var, *argv, dico, plus);
		else
			ft_print_export_error(*argv, var, &code_return);
		argv++;
	}
	return (code_return);
}
